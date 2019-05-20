#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent) {
	timer = new QTimer();
	timer->setInterval(3000);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    command_flag = false;

    tcpTimer = new QTimer();
    tcpTimer->setInterval(500);
    connect(tcpTimer, SIGNAL(timeout()), this, SLOT(tcpTimerTimeout()));
}

void TcpServer::startServer() {
    if (!this->listen(QHostAddress(ip), port))
	{
		qDebug() << "Could not start server";
	}
	else
	{
		qDebug() << "Listening ...";
    }
}

void TcpServer::setting(char IP[256], quint16 PORT)
{
    ip = IP;
    port = PORT;
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

	QByteArray txData;
	txData.append(QByteArray::fromRawData("\x03\x06", 2));
//	txData.append(65);
    txData.append(QByteArray::fromRawData("\x40\x00", 2));
	txData.append(QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00", 8));
	txData.append(QByteArray::fromRawData("\x0D\x05", 2));
	socket->write(txData);
    qDebug() << "Transmit Data(To UI) : " + txData.toHex();

    tcpTimer->start();
}	

void TcpServer::readyRead() {
	QByteArray rxData = socket->readAll();
    qDebug() << "Receive Data(From UI) : " + rxData.toHex();
    char *ch = new char[rxData.length()];
	for (int j = 0; j < rxData.length(); j++) {
		ch[j] = rxData.at(j);
	}
	if (ch[0] == 0x02 && ch[1] == 0x05) {
        if (ch[2] >= 0x01 && ch[2] <= 0x48) {
            qDebug() << "Product Number is " + QString::number(ch[2]);
            command_flag = true;
		}
        else if(ch[2] == 0 && command_flag == true){
            timer->start();
        }
	}
	delete[] ch;
}

void TcpServer::disconnected() {
	qDebug() << QString::number(socket->socketDescriptor()) + " Disconnected";
	socket->deleteLater();
    socket->close();
    tcpTimer->stop();
}

void TcpServer::timeout() {
	timer->stop();

    QByteArray txData;
    txData.append(QByteArray::fromRawData("\x03\x06", 2));
    txData.append(QByteArray::fromRawData("\x60\x00", 2));
    txData.append(QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00", 8));
    txData.append(QByteArray::fromRawData("\x0D\x05", 2));

    socket->write(txData);
    qDebug() << "Transmit Data(To UI) : " + txData.toHex();

    command_flag = false;
}

void TcpServer::tcpTimerTimeout(){

    tcpData.clear();
    tcpData.append(QByteArray::fromRawData("\x03\x06", 2));
    tcpData.append(QByteArray::fromRawData("\x00\x00", 2));
    tcpData.append(QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00", 8));
    tcpData.append(QByteArray::fromRawData("\x0D\x05", 2));

    socket->write(tcpData);
    qDebug() << "Transmit Data(To UI) : " + tcpData.toHex();
}
