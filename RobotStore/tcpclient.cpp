#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent) {
	socket = new QTcpSocket;
}

void TcpClient::connectToServer() {
	socket->connectToHost(ipAddress, portNum);
}

void TcpClient::setIpAddress(QString address) {
	ipAddress = address;
}

void TcpClient::setPort(quint16 num) {
	portNum = num;
}
