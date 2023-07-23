#include <QString>
#include <QtExamples/model.h>

namespace QtExamples
{
	Model::Model(QObject* parent /*= nullptr*/)
		: QAbstractItemModel(parent)
	{
		m_root = Item{};
		for (int i = 0; i < 5; ++i)
		{
			auto item = std::make_unique<Item>();
			item->name = std::to_string(i);
			item->parent = &m_root;
			for (auto j = 0; j < i + 1; j++)
			{
				auto kid = std::make_unique<Item>();
				kid->name = "child_i" + std::to_string(i) + "_j" + std::to_string(j);
				kid->parent = item.get();
				item->children.push_back(std::move(kid));
			}
			m_root.children.push_back(std::move(item));
		}
	}

	int Model::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const { return 1; }

	int Model::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
	{
		if (parent.isValid())
		{
			const Item* parentItem = static_cast<Item*>(parent.internalPointer());
			assert(parentItem);
			return static_cast<int>(parentItem->children.size());
		}
		else
		{
			return (int)m_root.children.size();
		}
	}


	QVariant Model::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
	{
		if (index.isValid() && role == Qt::DisplayRole)
		{
			Item* item = static_cast<Item*>(index.internalPointer());

			return QString::fromStdString(item->name);
		}
		return {};
	}


	QModelIndex Model::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
	{
		if (column != 0)
			return {};

		if (parent.isValid())
		{
			Item* parentItem = static_cast<Item*>(parent.internalPointer());
			return createIndex(row, 0, (void*)parentItem->children[row].get());
		}
		else
		{
			if (row >= 0 && row < int(m_root.children.size()))
			{
				return createIndex(row, 0, (void*)m_root.children[row].get());
			}
		}

		return {};
	}


	QModelIndex Model::parent(const QModelIndex& index) const
	{
		if (index.isValid())
		{
			auto parentItem = static_cast<Item*>(index.internalPointer())->parent;
			if (!parentItem)    // root
				return {};

			auto parentParentItem = parentItem->parent;
			if (!parentParentItem)
				return {};

			for (auto i = 0; i < parentParentItem->children.size(); ++i)
				if (parentParentItem->children[i].get() == parentItem)
					return createIndex(i, 0, (void*)parentItem);
		}
		return {};
	}
}
