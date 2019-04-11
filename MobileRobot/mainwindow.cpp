#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(500, 400);
    this->setWindowTitle("Mobile Robot");

    ui->ipAddress->setText("169.254.125.4");

    connectState = false;
    client = new TcpClient(this);

    connect(ui->connectBtn, SIGNAL(clicked()), this, SLOT(connectBtnSlot()));
    connect(client->socket, SIGNAL(connected()), this, SLOT(onConnectServer()));
    connect(client->socket, SIGNAL(readyRead()), this, SLOT(readMessage()));

    guestState = false;

    connect(ui->positionBtn, SIGNAL(clicked()), this, SLOT(positionBtnSlot()));
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

void MainWindow::connectBtnSlot() {
    if (connectState) {
        client->socket->close();
        ui->tcpMessage->append("Close ...");
        connectState = false;
        ui->connectBtn->setText("Connect");
    }
    else {
        client->setIpAddress(ui->ipAddress->text());
        emit client->connectToServer();
    }
}

void MainWindow::onConnectServer() {
    ui->tcpMessage->append("Connect complete ...");
    connectState = true;

    ui->connectBtn->setText("Disconnect");
}

void MainWindow::readMessage() {
    QString rxMessage;
    QByteArray rxData = client->socket->readAll();

    rxMessage = "Receive Data : " + rxData.toHex();
    ui->tcpMessage->append(rxMessage);

    char *ch = new char[rxData.length()];
    for (int j = 0; j < rxData.length(); j++) {
        ch[j] = rxData.at(j);
    }
    if (ch[0] == 0x02 && ch[1] == 0x05) {
        if (ch[2] == 0x01) {
            guestState = true;
            ui->guestCbox->setChecked(true);
            ui->positionBtn->setEnabled(true);
        }
        else if (ch[2] == 0) {
            guestState = false;
            ui->guestCbox->setChecked(false);
            ui->positionBtn->setDisabled(true);
        }
    }
}

void MainWindow::positionBtnSlot() {
    ui->positionBtn->setDisabled(true);

    QByteArray txData;
    txData.append(QByteArray::fromRawData("\x02\x05", 2));
    txData.append(QByteArray::fromRawData("\x02", 1));
    txData.append(QByteArray::fromRawData("\x0D\x05", 2));

    client->socket->write(txData);

    QString txMessage = "Transmit Data : " + txData.toHex();
    ui->tcpMessage->append(txMessage);
}
