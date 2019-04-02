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
    void setting(char IP[256], quint16 port);

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
    QString ip;
    quint16 port;
    bool command_flag;
};

