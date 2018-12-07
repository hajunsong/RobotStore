#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mobilerobot.h"
#include "tcpclient.h"

class MobileRobot : public QMainWindow
{
	Q_OBJECT

public:
	explicit MobileRobot(QWidget *parent = nullptr);
	~MobileRobot();

public slots:
	void connectBtnSlot();
	void onConnectServer();
	void readMessage();
	void positionBtnSlot();

private:
	Ui::MobileRobotClass ui;
	TcpClient *client;
	bool connectState, guestState;
};
