#pragma once

#include <QAbstractItemModel>
#include <QVariant>

#include <memory>

class QObject;

namespace QtExamples
{
	class Model : public QAbstractItemModel
	{
	public:
		explicit Model(QObject* parent = nullptr);

		// QAbstractItemModel interface implementation
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		QModelIndex parent(const QModelIndex& index) const override;
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;

		struct Item
		{
			std::string name;
			std::vector<std::unique_ptr<Item>> children;
			Item* parent = nullptr;
		};

		void appendChildren(Item* root, std::vector<Item*>& vec)
		{
			if (root->children.empty())
			{
				vec.push_back(root);
				return;
			}

			for (auto& i : root->children)
			{
				appendChildren(i.get(), vec);
			}
		}

		std::vector<Item*> getAll()
		{
			std::vector<Item*> items;
			appendChildren(&m_root, items);
			return items;
		}

		const Item& getRoot() const { return m_root; }

		int getRow(const Item* it)
		{
			for (int i = 0; i < rowCount(); ++i)
			{
				if (m_root.children[i].get() == it)
					return i;
			}
			return -1;
		}

	private:
		Item m_root;
	};
}