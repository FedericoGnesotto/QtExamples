#include <QtExamples/grouping_model.h>
#include <QtExamples/grouping_model_test.h>
#include <QtExamples/model.h>

#include <iostream>

void testGroupingModel()
{
	using namespace QtExamples;


	std::cout << "Test Grouping Model..\n";

	GroupingModel gModel{};
	Model model{};
	gModel.setSourceModel(&model);
	{
		auto fromSource = gModel.mapFromSource(model.index(0, 0));
		assert(fromSource.internalPointer() == model.index(0, 0).internalPointer());
	}

	{
		auto fromSource = gModel.mapFromSource(model.index(1, 0));
		assert(fromSource.internalPointer() == model.index(1, 0).internalPointer());
	}


	{
		auto fromSource = gModel.mapFromSource(model.index(2, 0));
		assert(fromSource.internalPointer() == model.index(2, 0).internalPointer());
	}


	{
		auto fromSource = gModel.mapFromSource(model.index(3, 0));
		assert(fromSource.internalPointer() == model.index(3, 0).internalPointer());
	}


	{
		auto toSource = gModel.mapToSource(gModel.index(3, 0));
		assert(!toSource.isValid());
	}

	{
		auto toSource = gModel.mapToSource(gModel.index(0, 0));
		assert(!toSource.isValid());
	}


	{
		auto even = gModel.index(0, 0);
		auto childInd = gModel.index(0, 0, even);
		assert(childInd.isValid());

		auto toSource = gModel.mapToSource(childInd);
		auto item = static_cast<Model::Item*>(toSource.internalPointer());
		assert(item->name == "0");
	}

	{
		auto odd = gModel.index(1, 0);
		auto childInd = gModel.index(0, 0, odd);
		assert(childInd.isValid());

		auto toSource = gModel.mapToSource(childInd);
		auto item = static_cast<Model::Item*>(toSource.internalPointer());
		assert(item->name == "1");
	}

	{
		auto odd = gModel.index(1, 0);
		auto childInd = gModel.index(1, 0, odd);
		assert(childInd.isValid());

		auto toSource = gModel.mapToSource(childInd);
		auto item = static_cast<Model::Item*>(toSource.internalPointer());
		assert(item->name == "3");
	}

	//data
	{
		auto data = gModel.data(gModel.index(0, 0));
		assert(data == "evens");
	}

	{
		auto data = gModel.data(gModel.index(1, 0));
		assert(data == "odds");
	}

	{
		auto data = gModel.data(gModel.index(0, 0, gModel.index(0, 0)), Qt::DisplayRole);    // second of the evens
		assert(data == "0");
	}
	{
		auto data = gModel.data(gModel.index(1, 0, gModel.index(0, 0)), Qt::DisplayRole);    // second of the evens
		assert(data == "2");
	}
	{
		auto data = gModel.data(gModel.index(2, 0, gModel.index(0, 0)), Qt::DisplayRole);    // second of the evens
		assert(data == "4");
	}


	{
		auto data = gModel.data(gModel.index(1, 0, gModel.index(1, 0)));    // second of the odds
		assert(data == "3");
	}

	{
		auto data = gModel.data(gModel.index(1, 0, gModel.index(1, 0, gModel.index(1, 0))));    // second of the second of the odds
		assert(data == "child_i3_j1");
	}

	//parent
	{
		assert(!gModel.parent({}).isValid());
		assert(!gModel.parent(gModel.index(1, 0)).isValid());

		assert(gModel.parent(gModel.index(1, 0, gModel.index(1, 0))) == gModel.index(1, 0));
		assert(gModel.parent(gModel.index(1, 0, gModel.index(0, 0))) == gModel.index(0, 0));
		assert(!gModel.parent(gModel.index(4, 0, gModel.index(0, 0))).isValid());

		assert(gModel.parent(gModel.index(0, 0, gModel.index(1, 0, gModel.index(0, 0)))) == gModel.index(1, 0, gModel.index(0, 0)));
	}

	{
		auto ind00 = gModel.index(0, 0, {});
		assert(gModel.data(ind00) == "evens");
		assert(gModel.rowCount(ind00) == 3);

		auto firstKidOf00 = static_cast<Model::Item*>(gModel.index(0, 0, ind00).internalPointer());
		assert(firstKidOf00->name == "0");
		assert(gModel.rowCount(gModel.index(0, 0, ind00)) == 1);

		auto secondKidOf00 = static_cast<Model::Item*>(gModel.index(1, 0, ind00).internalPointer());
		assert(secondKidOf00->name == "2");
		assert(gModel.data(gModel.index(1, 0, ind00), Qt::DisplayRole) == "2");

		assert(gModel.rowCount(gModel.index(1, 0, ind00)) == 3);
		auto firstKidOf2 = static_cast<Model::Item*>(gModel.index(0, 0, gModel.index(1, 0, ind00)).internalPointer());
		assert(firstKidOf2->name == "child_i2_j0");
		auto secondKidOf2 = static_cast<Model::Item*>(gModel.index(1, 0, gModel.index(1, 0, ind00)).internalPointer());
		assert(secondKidOf2->name == "child_i2_j1");

		auto thirdKidOf00 = static_cast<Model::Item*>(gModel.index(2, 0, ind00).internalPointer());
		assert(thirdKidOf00->name == "4");
	}

	{
		auto ind10 = gModel.index(1, 0, {});
		assert(gModel.data(ind10) == "odds");
		auto firstKidOf10 = static_cast<Model::Item*>(gModel.index(0, 0, ind10).internalPointer());
		assert(firstKidOf10->name == "1");

		auto secondKidOf10 = static_cast<Model::Item*>(gModel.index(1, 0, ind10).internalPointer());
		assert(secondKidOf10->name == "3");

		auto thirdKidOf10 = static_cast<Model::Item*>(gModel.index(2, 0, ind10).internalPointer());
		assert(thirdKidOf10 == nullptr);
	}

	{
		auto ind10 = gModel.index(1, 0, {});
		assert(gModel.rowCount(ind10) == 2);

		auto item = static_cast<Model::Item*>(gModel.index(1, 0, ind10).internalPointer());
		auto rows = gModel.rowCount(gModel.index(1, 0, ind10));
		assert(rows == 4);
	}
}