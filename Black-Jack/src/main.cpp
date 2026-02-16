// Black-Jack.cpp : Defines the entry point for the application.
//

#include <QApplication>
#include "mainwindow.h"
#include "Black-Jack.h"

using namespace std;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow window;
	window.show();
	return app.exec();
}
