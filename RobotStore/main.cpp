#include "robotstore.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	RobotStore w;
	//w.show();
	w.showFullScreen();
	
	return a.exec();
}
