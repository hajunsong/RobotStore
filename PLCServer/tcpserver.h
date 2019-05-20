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
    void tcpTimerTimeout();

protected:
	void incomingConnection(qintptr socketDescriptor);

private:
	QTcpSocket *socket;
    QTimer *timer, *tcpTimer;
    QString ip;
    quint16 port;
    bool command_flag;
    QByteArray tcpData;
};

