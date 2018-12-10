#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent) {
	timer = new QTimer();
	timer->setInterval(3000);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void TcpServer::startServer() {
	if (!this->listen(QHostAddress::Any, 9999))
	{
		qDebug() << "Could not start server";
	}
	else
	{
		qDebug() << "Listening ...";
	}
}

void TcpServer::incomingConnection(qintptr socketDescriptor) {
	// We have a new connection
	qDebug() << QString::number(socketDescriptor) + " Connecting...";

	socket = new QTcpSocket();

	// set the ID
	if (!socket->setSocketDescriptor(socketDescriptor))
	{
		// something's wrong, we just emit a signal
		emit error(socket->error());
		return;
	}

	// connect socket and signal
	// note - Qt::DirectConnection is used because it's multithreaded
	//        This makes the slot to be invoked immediately, when the signal is emitted.

	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);
}

void TcpServer::readyRead() {
	QByteArray rxData = socket->readAll();
	qDebug() << "Receive Data(From UI) : " + rxData;
	QChar *ch = new QChar[rxData.length()];
	for (int j = 0; j < rxData.length(); j++) {
		ch[j] = rxData.at(j);
	}
	if (ch[0] == 0x02 && ch[1] == 0x05) {
		if (ch[2] >= 0x01 && ch[2] <= 0x2A) {
			qDebug() << "Product Number is " + QString(ch[2]).toUtf8();
			timer->start();
		}
	}
	delete[] ch;
}

void TcpServer::disconnected() {
	qDebug() << QString::number(socket->socketDescriptor()) + " Disconnected";
	socket->deleteLater();
}

void TcpServer::timeout() {
	timer->stop();

	QByteArray txData;
	txData.append(QByteArray::fromHex("0306"));
	txData.append(QByteArray::fromHex("0002"));
	txData.append(QByteArray::fromHex("000000000000000000"));
	txData.append(QByteArray::fromHex("0D05"));

	socket->write(txData);
	qDebug() << "Transmit Data(To UI) : " + txData;
}