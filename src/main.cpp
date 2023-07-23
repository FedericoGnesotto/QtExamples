#include <QApplication>
#include <QMainWindow>
#include <QTreeView>
#include <QtExamples/grouping_model.h>
#include <QtExamples/grouping_model_test.h>
#include <QtExamples/model.h>

#include <iostream>
#include <vector>
#include <xutility>

int main(int argc, char* argv[])
{
	std::vector<std::string> args(argv, argv + argc);
	if (args.size() == 1)
	{
		QApplication app(argc, argv);
		QMainWindow mw{};
		QTreeView* view = new QTreeView();
		auto model = new QtExamples::Model(view);
		auto proxy_model = new QtExamples::GroupingModel();
		proxy_model->setSourceModel(model);
		view->setModel(proxy_model);
		view->setHeaderHidden(true);
		mw.setCentralWidget(view);
		view->expandAll();
		mw.show();
		app.exec();
		return 0;
	}
	else if (args[1] == "--test")
	{
		testGroupingModel();
		std::cout << "Test OK!" << std::endl;
		return 0;
	}
}