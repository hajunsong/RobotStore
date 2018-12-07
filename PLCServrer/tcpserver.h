#pragma once

#include <QTcpServer>
#include <QDebug>
#include <QTcpSocket>
#include <QTimer>

class TcpServer : public QTcpServer
{
	Q_OBJECT
public:
	explicit TcpServer(QObject *parent = nullptr);
	void startServer();

signals:
	void error(QTcpSocket::SocketError socketerror);

public slots:
	void readyRead();
	void disconnected();
	void timeout();

protected:
	void incomingConnection(qintptr socketDescriptor);

private:
	QTcpSocket *socket;
	QTimer *timer;
};

