#include <QtCore/QCoreApplication>
#include "tcpserver.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	TcpServer *server = new TcpServer();
	server->startServer();

	return a.exec();
}
