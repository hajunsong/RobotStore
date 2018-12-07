#include "mobilerobot.h"

MobileRobot::MobileRobot(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	this->setFixedSize(500, 400);
	this->setWindowTitle("Mobile Robot");

	ui.ipAddress->setText("127.0.0.1");

	connectState = false;
	client = new TcpClient(this);

	connect(ui.connectBtn, SIGNAL(clicked()), this, SLOT(connectBtnSlot()));
	connect(client->socket, SIGNAL(connected()), this, SLOT(onConnectServer()));
	connect(client->socket, SIGNAL(readyRead()), this, SLOT(readMessage()));

	guestState = false;

	connect(ui.positionBtn, SIGNAL(clicked()), this, SLOT(positionBtnSlot()));
}

MobileRobot::~MobileRobot() {
	delete client;
}

void MobileRobot::connectBtnSlot() {
	if (connectState) {
		client->socket->close();
		ui.tcpMessage->append("Close ...");
		connectState = false;
		ui.connectBtn->setText("Connect");
	}
	else {
		client->setIpAddress(ui.ipAddress->text());
		emit client->connectToServer();
	}
}

void MobileRobot::onConnectServer() {
	ui.tcpMessage->append("Connect complete ...");
	connectState = true;

	ui.connectBtn->setText("Disconnect");
}

void MobileRobot::readMessage() {
	QString rxMessage;
	QString rxData = client->socket->readAll();

	rxMessage = "Receive Data : " + rxData;
	ui.tcpMessage->append(rxMessage);

	if (rxData.toInt() == 1) {
		guestState = true;
		ui.guestCbox->setChecked(true);
		ui.positionBtn->setEnabled(true);
	}
	else if (rxData.toInt() == 0) {
		guestState = false;
		ui.guestCbox->setChecked(false);
		ui.positionBtn->setDisabled(true);
	}
}

void MobileRobot::positionBtnSlot() {
	ui.positionBtn->setDisabled(true);
	QString txData = QString::number(2);
	client->socket->write(txData.toUtf8());

	QString txMessage = "Transmit Data : " + txData;
	ui.tcpMessage->append(txMessage);
}
