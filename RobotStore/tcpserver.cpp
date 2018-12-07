#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent) {
}

void TcpServer::startServer() {
	if (!this->listen(QHostAddress(ipAddress), portNum)) {
		qDebug() << "Could not start server";
	}
	else {
		qDebug() << "Listening...";
	}
}

void TcpServer::setIpAddress(QString address) {
	ipAddress = address;
}

void TcpServer::setPort(quint16 num) {
	portNum = num;
}

void TcpServer::incomingConnection(qintptr socketDescriptor) {
	// We have a new connection
	qDebug() << QString::number(socketDescriptor) + " Connecting...";

	socket = new QTcpSocket();

	// set the ID
	if (!socket->setSocketDescriptor(socketDescriptor)) {
		// something's wrong, we just emit a signal
		emit error(socket->error());
		return;
	}

	emit connectedClient();
	// connect socket and signal
	// note - Qt::DirectConnection is used because it's multithreaded
	//        This makes the slot to be invoked immediately, when the signal is emitted.

	//connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
	//connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);

}
