// Black-Jack.cpp : Defines the entry point for the application.
#include "mainwindow.h"

#include <iostream>
#include <QApplication>
#include <QMessageBox>
#include <QLabel>

using namespace std;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QMessageBox errMsg;
	std::filesystem::path currentPath = std::filesystem::current_path();
	std::string errorMessage = "";
	bool foundFolder = false;
	QPixmap CardBacksidePixmap;
	try {
		for (const auto& entry : std::filesystem::recursive_directory_iterator(currentPath)) {
			if (entry.is_directory() && entry.path().filename() == "spillekort") {
				std::filesystem::path backPicturePath = entry.path();
				backPicturePath += std::filesystem::path("\\Backside.png");
				CardBacksidePixmap.load(QString::fromStdString(backPicturePath.string()));

				if (CardBacksidePixmap.isNull() == true) {
					errorMessage = "Kunne ikke loade billede: " + backPicturePath.string();
					break;
				}
				foundFolder = true;
				break;
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		errorMessage = "Filesystem fejl: " + std::string(e.what());
	}

	if (errorMessage.empty() == false)
	{
		errMsg.setText(QString::fromStdString(errorMessage));
		return errMsg.exec();
	}
	else if (foundFolder == false)
	{
		errMsg.setText("Mappen 'spillekort' blev ikke fundet.");
		return errMsg.exec();
	}
	else
	{
		MainWindow window(CardBacksidePixmap);
		window.show();
		return app.exec();
	}
}
