#pragma once

#include <QTcpServer>
#include <QDebug>
#include <QTcpSocket>

class TcpServer : public QTcpServer
{
	Q_OBJECT
public:
	explicit TcpServer(QObject *parent = nullptr);
	QTcpServer *server;
	QTcpSocket *socket;
	void startServer();
	void setIpAddress(QString address);
	void setPort(quint16 num);

signals:
	void error(QTcpSocket::SocketError socketerror);
	void connectedClient();

protected:
	void incomingConnection(qintptr socketDescriptor);

private:
	QString ipAddress;
	quint16 portNum;
};

