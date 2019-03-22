#include "robotstore.h"

RobotStore::RobotStore(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);
	this->setFixedSize(1920, 1080);
	this->setWindowTitle("Robot Store");

	ui.plcIP->setText("192.168.100.4");
	ui.serverIP->setText("192.168.173.1");
	ui.plcPort->setText("5003");
	ui.serverPort->setText("8888");

	connect(ui.listenBtn, SIGNAL(clicked()), this, SLOT(listenBtnSlot()));
	connect(ui.connectBtn, SIGNAL(clicked()), this, SLOT(connectBtnSlot()));

	MR = new TcpServer();
	PLC = new TcpClient();

	connectStateMR = false;
	connectStatePLC = false;
	PLCReady = false;

	connect(PLC->socket, SIGNAL(connected()), this, SLOT(onConnectPLC()));
	connect(PLC->socket, SIGNAL(readyRead()), this, SLOT(readMessageFromPLC()));

	connect(ui.uiStartBtn, SIGNAL(clicked()), this, SLOT(uiStartBtnSlot()));
	ui.uiStartBtn->setDisabled(true);

	timer = new QTimer(this);
	timer->setInterval(3000);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

	mainUI = new SlidingStackedWidget(this);
	mainUI->hide();
	mainUI->setGeometry(this->rect());

	quint64 pages = 4;
	pageWidget = new QWidget[pages];
	for (quint64 i = 0; i < pages; i++) {
		pageWidget[i].setGeometry(mainUI->rect());
		mainUI->addWidget(&pageWidget[i]);
	}
	mainUI->setVerticalMode(true);
	mainUI->setSpeed(500);

	// 1 Page
	startBtn = new QClickLabel(&pageWidget[startPage]);
	startBtn->setGeometry(pageWidget[startPage].rect());
	startBtn->setObjectName(QStringLiteral("startBtn"));
	connect(startBtn, SIGNAL(mousePressed()), this, SLOT(startBtnPressedSlot()));
	connect(startBtn, SIGNAL(mouseReleased()), this, SLOT(startBtnReleasedSlot()));
	clickLabelDrawImage(startBtn, imageHeader + startIcon[0], 0.3);

	resetBtn = new QPushButton(&pageWidget[startPage]);
	int resetBtnWidth = 100, resetBtnHeight = 70;
	resetBtn->setGeometry(pageWidget[startPage].width() - resetBtnWidth, pageWidget[startPage].height() - resetBtnHeight, resetBtnWidth, resetBtnHeight);
	resetBtn->setObjectName(QStringLiteral("resetBtn"));
	resetBtn->hide();
	connect(resetBtn, SIGNAL(clicked()), this, SLOT(resetBtnSlot()));

	clerkLabel = new QLabel(&pageWidget[startPage]);
	clerkLabel->setGeometry(pageWidget[startPage].rect());
	labelDrawImage(clerkLabel, imageHeader + clerkIcon, 0.2);
	clerkLabel->hide();

	// 2 Page
	int pageWidth = pageWidget[selectPage].width(), pageHeight = pageWidget[selectPage].height();
	for (int i = 0; i < sizeColor; i++) {
		colorBtn[i] = new QClickLabel(&pageWidget[selectPage]);
		colorBtn[i]->setObjectName("Color " + QString::number(i + 1));
		colorBtn[i]->setGeometry(pageWidth*(0.04 + i * 0.15), pageHeight / 3 * 0.3, pageWidth / 6, pageHeight / 3);
		connect(colorBtn[i], SIGNAL(mousePressed()), this, SLOT(colorBtnPressedSlot()));
		connect(colorBtn[i], SIGNAL(mouseReleased()), this, SLOT(combineItem()));
	}

	for (int i = 0; i < sizePattern; i++) {
		patternBtn[i] = new QClickLabel(&pageWidget[selectPage]);
		patternBtn[i]->setObjectName("Pattern " + QString::number(i + 1));
		patternBtn[i]->setGeometry(pageWidth*(0.035 + i * 0.13), pageHeight / 3 * 1.2, pageWidth / 7, pageHeight / 3);
		connect(patternBtn[i], SIGNAL(mousePressed()), this, SLOT(patternBtnPressedSlot()));
		connect(patternBtn[i], SIGNAL(mouseReleased()), this, SLOT(combineItem()));
	}

	bottomLayout = new QWidget(&pageWidget[selectPage]);
	bottomLayout->setGeometry(0, pageHeight / 5 * 4, pageWidth, pageHeight / 5);
	orderBtn = new QClickLabel(bottomLayout);
	orderBtn->setObjectName("orderBtn");
	connect(orderBtn, SIGNAL(mousePressed()), this, SLOT(orderBtnPressedSlot()));
	connect(orderBtn, SIGNAL(mouseReleased()), this, SLOT(orderBtnReleasedSlot()));

	// 3 Page
	itemLabel = new QLabel(&pageWidget[itemPage]);
	itemLabel->setGeometry(pageWidth*0.2, pageHeight*0.2, pageWidth*0.6, pageHeight*0.6);
	itemLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

	waitLabel = new QLabel(&pageWidget[itemPage]);
	waitLabel->setGeometry(pageWidth*0.2, pageHeight*0.8, pageWidth*0.6, pageHeight*0.2);
	labelDrawImage(waitLabel, imageHeader + waitIcon, 0.12);

	// 4 Page
	thankLabel = new QLabel(&pageWidget[thankPage]);
	thankLabel->setGeometry(pageWidget[thankPage].rect());
	labelDrawImage(thankLabel, imageHeader + thankIcon, 0.7);

	selectPageInit();

	robotPositionState = false;

	delayTimer = new QTimer(this);
	delayTimer->setInterval(3000);
	connect(delayTimer, SIGNAL(timeout()), this, SLOT(delayTimeout()));

	move_flag = false;
	start_flag = false;
	single_flag = false;

	connect(ui.cbSingleTest, SIGNAL(clicked()), this, SLOT(singleCbSlot()));
}

RobotStore::~RobotStore() {
	delete MR;
	delete PLC;
	delete timer;

	delete startBtn;
	delete resetBtn;

	for (int i = 0; i < sizeColor; i++) {
		delete colorBtn[i];
	}
	for (int i = 0; i < sizePattern; i++) {
		delete patternBtn[i];
	}
	delete orderBtn;
	delete bottomLayout;
	delete clerkLabel;

	delete itemLabel;
	delete waitLabel;

	delete thankLabel;

	delete[] pageWidget;
	delete mainUI;

	delete delayTimer;
}

void RobotStore::selectPageInit() {
	for (int i = 0; i < 6; i++) clickLabelDrawImage(colorBtn[i], imageHeader + colorIcon[i], 0.35);
	for (int i = 0; i < 7; i++) clickLabelDrawImage(patternBtn[i], imageHeader + patternIcon[i], 0.35);

	orderBtn->setGeometry(bottomLayout->width() / 6, 0, bottomLayout->width() / 3 * 2, bottomLayout->height() / 5 * 4);
	clickLabelDrawImage(orderBtn, imageHeader + orderIcon[0], 0.2);

	colorIndex = -1;
	patternIndex = -1;
}

void RobotStore::listenBtnSlot() {
	MR->setIpAddress(ui.serverIP->text());
	MR->setPort(ui.serverPort->text().toUShort());
	MR->startServer();

	connect(MR, SIGNAL(connectedClient()), this, SLOT(connectedClient()));

	colorIndex = -1;
	patternIndex = -1;

	ui.listenBtn->setDisabled(true);
}

void RobotStore::connectBtnSlot() {
	if (connectStatePLC) {
		PLC->socket->close();
		ui.tcpMessage->append("PLC Server Close ...");
		connectStatePLC = false;
		ui.connectBtn->setText("Connect");
		ui.uiStartBtn->setDisabled(true);
	}
	else {
		PLC->setIpAddress(ui.plcIP->text());
		PLC->setPort(ui.plcPort->text().toUShort());
		emit PLC->connectToServer();
	}
}

void RobotStore::onConnectPLC() {
	ui.tcpMessage->append("PLC Server Connect complete ...");
	connectStatePLC = true;
	ui.connectBtn->setText("Disconnect");
	checkAllConnectState();
}

void RobotStore::readMessageFromPLC() {
	QString rxMessage;
	QByteArray rxData = PLC->socket->readAll();

	rxMessage = "Receive Data(From PLC) : " + rxData;
	ui.tcpMessage->append(rxMessage);

	char *ch = new char[rxData.length()];
	for (int j = 0; j < rxData.length(); j++) {
		ch[j] = rxData.at(j);
	}
	qDebug() << "Receive Data(From PLC) : " + rxData;
	
	if (PLCReady) {
		if (ch[0] == 0x03 && ch[1] == 0x06) {
			if (ch[2] == 0x50) 
			//if ((ch[2] & 0b00010000) == 0b00010000)
			{
				qDebug() << "Manipulator operating";
			}
			else if (ch[2] == 0x60  && move_flag)
			//else if ((ch[2] & 0b00100000) == 0b00100000 && move_flag)
			{
				qDebug() << "Manipulator operation complete";
				mainUI->slideInIdx(thankPage, mainUI->TOP2BOTTOM);
				timer->start();

				if (!single_flag) {
					QByteArray txDataMR;
					txDataMR.append(QByteArray::fromRawData("\x02\x05", 2));
					txDataMR.append(QByteArray::fromRawData("\x00", 1));
					txDataMR.append(QByteArray::fromRawData("\x0D\x05", 2));
					MR->socket->write(txDataMR);
					qDebug() << "Transmit Data(To MR) : " + txDataMR;
				}

				move_flag = false;
			}
		}
	}
	else {
		if (ch[0] == 0x03 && ch[1] == 0x06) {
			if ((ch[2] & 0b01000000) == 0b01000000) 
			//if (ch[2] == 0x60)
			{
				qDebug() << "Manipulator ready!!";
				PLCReady = true;
			}
		}
	}
	delete[] ch;
}

void RobotStore::uiStartBtnSlot() {

	ui.centralWidget->hide();
	mainUI->setHidden(false);
}

void RobotStore::timeout() {
	timer->stop();
	mainUI->slideInIdx(startPage, mainUI->TOP2BOTTOM);
	selectPageInit();
	if (!single_flag)
		robotPositionState = false;
}

void RobotStore::startBtnPressedSlot() {
	clickLabelDrawImage(startBtn, imageHeader + startIcon[1], 0.3);
}

void RobotStore::startBtnReleasedSlot() {
	clerkLabel->show();

	if (!single_flag) {
		QByteArray txDataMR;
		txDataMR.append(QByteArray::fromRawData("\x02\x05", 2));
		txDataMR.append(QByteArray::fromRawData("\x01", 1));
		txDataMR.append(QByteArray::fromRawData("\x0D\x05", 2));
		MR->socket->write(txDataMR);
		qDebug() << "Transmit Data(To MR) : " + txDataMR;
	}
	else {
		clerkLabel->hide();

		clickLabelDrawImage(startBtn, imageHeader + startIcon[0], 0.3);
		mainUI->slideInIdx(selectPage, mainUI->BOTTOM2TOP);
	}
}

void RobotStore::resetBtnSlot() {
	ui.centralWidget->setHidden(false);
	mainUI->hide();
	selectPageInit();
}

void RobotStore::colorBtnPressedSlot() {
	//qDebug() << sender()->objectName();
	int index = sender()->objectName().split(" ")[1].toInt() - 1;
	for (int i = 0; i < sizeColor; i++) {
		QString selectedItem = "";
		if (i == index) {
			colorIndex = index;
			clickLabelDrawImage(colorBtn[i], imageHeader + "m" + QString::number(i + 1), 0.35);
		}
		else {
			clickLabelDrawImage(colorBtn[i], imageHeader + colorIcon[i], 0.35);
		}
	}
}

void RobotStore::patternBtnPressedSlot() {
	int index = sender()->objectName().split(" ")[1].toInt() - 1;
	for (int i = 0; i < sizePattern; i++) {
		QString selectedItem = "";
		if (i == index) {
			patternIndex = index;
			clickLabelDrawImage(patternBtn[i], imageHeader + "gb" + QString::number(i + 1), 0.35);
		}
		else {
			clickLabelDrawImage(patternBtn[i], imageHeader + patternIcon[i], 0.35);
		}
	}
}

void RobotStore::orderBtnPressedSlot() {
	if (colorIndex >= 0 && patternIndex >= 0) {
		clickLabelDrawImage(orderBtn, imageHeader + orderIcon[2], 0.2);
	}
}

void RobotStore::orderBtnReleasedSlot() {
	if (colorIndex >= 0 && patternIndex >= 0) {
		clickLabelDrawImage(orderBtn, imageHeader + orderIcon[1], 0.2);

		mainUI->slideInIdx(itemPage, mainUI->BOTTOM2TOP);
		int itemIndex = colorIndex * sizePattern + patternIndex;
		labelDrawImage(itemLabel, imageHeader + imageIcon[itemIndex], 4);

		QByteArray txData;
		txData.append(QByteArray::fromRawData("\x02\x05", 2));
		txData.append(2*(itemIndex + 1) - 1);
		txData.append(QByteArray::fromRawData("\x01", 1));
		txData.append(QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00", 8));
		txData.append(QByteArray::fromRawData("\x0D\x05", 2));
		PLC->socket->write(txData);
		qDebug() << "Transmit Data(To PLC) : " + txData;

		if (!single_flag) {
			QByteArray txDataMR;
			txDataMR.append(QByteArray::fromRawData("\x02\x05", 2));
			txDataMR.append(QByteArray::fromRawData("\x02", 1));
			txDataMR.append(QByteArray::fromRawData("\x0D\x05", 2));

			MR->socket->write(txDataMR);
			qDebug() << "Transmit Data(To MR) : " + txDataMR;
		}

		delayTimer->start();
	}
}

void RobotStore::checkAllConnectState() {
	if (connectStateMR && PLCReady) {
		ui.uiStartBtn->setEnabled(true);
	}
	else {
		ui.uiStartBtn->setDisabled(true);
	}
	if (single_flag) {
		if (PLCReady) {
			ui.uiStartBtn->setEnabled(true);
			robotPositionState = true;
		}
		else {
			ui.uiStartBtn->setDisabled(true);
		}
	}
}

void RobotStore::combineItem() {
	if (colorIndex >= 0 && patternIndex >= 0) {
		orderBtn->setGeometry(bottomLayout->width() / 3, 0, bottomLayout->width() / 3, bottomLayout->height() / 5 * 4);
		clickLabelDrawImage(orderBtn, imageHeader + orderIcon[1], 0.2);
	}
}

void RobotStore::clickLabelDrawImage(QClickLabel *clickLabel, QString imagePath, double scale) {
	QImage *image = new QImage();
	QPixmap *buffer = new QPixmap();
	if (image->load(imagePath)) {
		*buffer = QPixmap::fromImage(*image);
		*buffer = buffer->scaled(image->width()*scale, image->height()*scale);
	}
	clickLabel->setPixmap(*buffer);
	clickLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
}

void RobotStore::labelDrawImage(QLabel *label, QString imagePath, double scale) {
	QImage *image = new QImage();
	QPixmap *buffer = new QPixmap();
	if (image->load(imagePath)) {
		*buffer = QPixmap::fromImage(*image);
		*buffer = buffer->scaled(image->width()*scale, image->height()*scale);
	}
	label->setPixmap(*buffer);
	label->setAlignment(Qt::AlignmentFlag::AlignCenter);
}

void RobotStore::connectedClient() {
	connectStateMR = true;
	connect(MR->socket, SIGNAL(readyRead()), this, SLOT(readMessageFromMR()), Qt::DirectConnection);
	connect(MR->socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);
	checkAllConnectState();
}

void RobotStore::readMessageFromMR() {
	QByteArray rxData = MR->socket->readAll();
	QString rxMessage;

	qDebug() << "Receive Data(From MR) : " + rxData;
	QChar *ch = new QChar[rxData.length()];
	for (int j = 0; j < rxData.length(); j++) {
		ch[j] = rxData.at(j);
	}
	if (ch[0] == 0x02 && ch[1] == 0x05) {
		if (ch[2] == 0x02) {
			qDebug() << "Mobile robot arrives in front of guest";
			robotPositionState = true;
			clerkLabel->hide();

			clickLabelDrawImage(startBtn, imageHeader + startIcon[0], 0.3);
			mainUI->slideInIdx(selectPage, mainUI->BOTTOM2TOP);

			QByteArray txData;
			txData.append(QByteArray::fromRawData("\x02\x05", 2));
			txData.append(QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10));
			txData.append(QByteArray::fromRawData("\x0D\x05", 2));

			PLC->socket->write(txData);
			qDebug() << "Transmit Data(To PLC) : " + txData;
		}
	}
}

void RobotStore::disconnected() {
	connectStateMR = false;
	qDebug() << QString::number(MR->socket->socketDescriptor()) + " Disconnected";
	MR->socket->deleteLater();
}

void RobotStore::delayTimeout() {
	QByteArray txData;
	txData.append(QByteArray::fromRawData("\x02\x05", 2));
	txData.append(QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10));
	txData.append(QByteArray::fromRawData("\x0D\x05", 2));

	PLC->socket->write(txData);
	qDebug() << "Transmit Data(To PLC) : " + txData;

	delayTimer->stop();

	move_flag = true;
}

void RobotStore::singleCbSlot() {
	single_flag = ui.cbSingleTest->isChecked();
}