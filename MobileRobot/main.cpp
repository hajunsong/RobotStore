#include "mobilerobot.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MobileRobot w;
	w.show();
	return a.exec();
}
