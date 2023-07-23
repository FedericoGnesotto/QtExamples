
#include <QtExamples/grouping_model.h>

#include <iostream>

namespace QtExamples
{

	QModelIndex GroupingModel::mapFromSource(const QModelIndex& sourceIndex) const
	{
		if (!sourceIndex.isValid())
			return {};

		if (sourceIndex.parent().internalPointer() == nullptr)
		{
			auto proxyRow = sourceIndex.row() / 2;
			return createIndex(proxyRow, sourceIndex.column(), sourceIndex.internalPointer());
		}

		return createIndex(sourceIndex.row(), 0, sourceIndex.internalPointer());
	}

	QModelIndex GroupingModel::mapToSource(const QModelIndex& proxyIndex) const
	{
		assert(!proxyIndex.isValid() || proxyIndex.model() == this);

		if (!proxyIndex.internalPointer() || !proxyIndex.isValid())    // no match in source
		{
			return {};
		}

		return static_cast<Helper*>(sourceModel())->createIndex(proxyIndex.row(), proxyIndex.column(), proxyIndex.internalPointer());
	}

	void GroupingModel::setSourceModel(Model* model)
	{
		if (sourceModel())
			disconnect(sourceModel(), 0, this, 0);
		QAbstractProxyModel::setSourceModel(model);
		if (!sourceModel())
			return;

		createGroups();
	}


	QModelIndex GroupingModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
	{
		if (hasIndex(row, column, parent))
		{
			if (!parent.isValid())    // parent is root
			{
				return createIndex(row, 0);
			}
			else if (parent.internalPointer() == nullptr)    // parent is in even/odd group
			{
				int sourceRow = m_groupItems[parent.row()].childrenRows[row].second;
				auto sourceIndex = sourceModel()->index(sourceRow, column);
				auto proxyRow = sourceIndex.row() / 2;
				return createIndex(proxyRow, sourceIndex.column(), sourceIndex.internalPointer());
			}
			else    // parent should be in source
			{
				QModelIndex sourceParent = mapToSource(parent);
				QModelIndex sourceIndex = sourceModel()->index(row, 0, sourceParent);
				assert(sourceIndex.isValid());
				return mapFromSource(sourceIndex);
			}
		}
		return QModelIndex();
	}

	QModelIndex GroupingModel::parent(const QModelIndex& child) const
	{
		if (!child.isValid())
			return QModelIndex();

		if (!child.internalPointer())
			return {};

		auto childSource = mapToSource(child);
		auto sourceParent = sourceModel()->parent(childSource);
		if (!sourceParent.isValid())
		{
			auto childItem = static_cast<Model::Item*>(childSource.internalPointer());
			return createIndex(std::stoi(childItem->name) % 2 == 0 ? 0 : 1, 0);
		}

		return mapFromSource(sourceParent);
	}


	int GroupingModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
	{
		if (!parent.isValid())
			return 2;


		if (auto item = static_cast<Model::Item*>(parent.internalPointer()); !item)
		{
			return m_groupItems[parent.row()].childrenRows.size();
		}
		else
		{
			return item->children.size();
		}
	}

	int GroupingModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const { return 1; }

	QVariant GroupingModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
	{
		if (index.isValid() && role == Qt::DisplayRole)
		{
			if (index.internalPointer() == nullptr)
				return QString::fromStdString(m_groupItems[index.row()].name);

			return sourceModel()->data(mapToSource(index), role);
		}
		return {};
	}


	void GroupingModel::createGroups()
	{
		if (!sourceModel())
			return;

		auto& kids = static_cast<Model*>(sourceModel())->getRoot().children;
		std::vector<std::pair<Model::Item*, int>> allData;
		std::transform(kids.begin(), kids.end(), std::back_inserter(allData), [this](auto& it) {
			return std::pair<Model::Item*, int>{it.get(), static_cast<Model*>(sourceModel())->getRow(it.get())};
		});

		std::vector<std::pair<Model::Item*, int>> evens, odds;
		std::partition_copy(
			allData.begin(), allData.end(), std::back_inserter(evens), std::back_inserter(odds), [](std::pair<Model::Item*, int> const& itemPair) {
				return std::stoi(itemPair.first->name) % 2 == 0;
			});

		m_groupItems.emplace_back(GroupItem{"evens", evens});
		m_groupItems.emplace_back(GroupItem{"odds", odds});
	}
}