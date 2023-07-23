#pragma once

#include <QAbstractProxyModel>
#include <QtExamples/model.h>

#include <string>
#include <utility>
#include <vector>

namespace QtExamples
{

	class GroupingModel : public QAbstractProxyModel
	{
	public:
		QModelIndex mapFromSource(const QModelIndex& sourceIndex) const override;
		QModelIndex mapToSource(const QModelIndex& proxyIndex) const override;

		void setSourceModel(Model* model);

		QModelIndex createSourceIndex(int r, int c, void* data) const { return static_cast<Helper*>(sourceModel())->createIndex(r, c, data); }

		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		QModelIndex parent(const QModelIndex& child) const override;
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	private:
		struct Helper : QAbstractItemModel
		{
			using QAbstractItemModel::createIndex;
		};

		struct GroupItem
		{
			std::string name;
			std::vector<std::pair<Model::Item*, int>> childrenRows;
		};

		void createGroups();
		std::vector<GroupItem> m_groupItems;
	};
}
