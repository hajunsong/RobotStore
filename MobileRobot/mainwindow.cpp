#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(500, 400);
    this->setWindowTitle("Mobile Robot");

    ui->ipAddress->setText("127.0.0.1");

    connectState = false;
    client = new TcpClient(this);

    connect(ui->connectBtn, SIGNAL(clicked()), this, SLOT(connectBtnSlot()));
    connect(client->socket, SIGNAL(connected()), this, SLOT(onConnectServer()));
    connect(client->socket, SIGNAL(readyRead()), this, SLOT(readMessage()));

    guestState = false;

    connect(ui->positionBtn, SIGNAL(clicked()), this, SLOT(positionBtnSlot()));
    connect(ui->btnDockingStation, SIGNAL(clicked()), this, SLOT(btnDockingStationSlot()));

    ui->positionBtn->setEnabled(false);
    ui->guestCbox->setChecked(false);
    ui->btnDockingStation->setEnabled(false);
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

    char rxHead = 0x02, rxHeadSub = 0x05;
    QList<QByteArray> rxDataList = rxData.split(rxHead);
    int len = rxDataList.length();
    for(int i = 0; i < len; i++){
        QList<QByteArray> rxDataListSub = rxDataList[i].split(rxHeadSub);
        int len_sub = rxDataListSub.length();
        if (len_sub > 1){
            for(int j = 0; j < len_sub; j++){
                if(rxDataListSub[j].length() > 0){
                    int data = rxDataListSub[j].at(0);
                    qDebug() << "data : " + QString::number(data);
                    switch(data){
                    case 0:
                        ui->positionBtn->setDisabled(true);
                        ui->btnDockingStation->setEnabled(true);
                        break;
                    case 1:
                        guestState = true;
                        ui->guestCbox->setChecked(true);
                        ui->positionBtn->setEnabled(true);
                        ui->btnDockingStation->setEnabled(false);
                        break;
                    case 3:
                        guestState = false;
                        ui->guestCbox->setChecked(false);
                        break;
                    case 7:
                        guestState = false;
                        ui->guestCbox->setChecked(false);
                        ui->btnDockingStation->setEnabled(true);
                        break;
                    default:
                        break;
                    }
                }
            }
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

void MainWindow::btnDockingStationSlot(){
    QByteArray txData;
    txData.append(QByteArray::fromRawData("\x02\x05", 2));
    txData.append(QByteArray::fromRawData("\x05", 1));
    txData.append(QByteArray::fromRawData("\x0D\x05", 2));

    client->socket->write(txData);

    QString txMessage = "Transmit Data : " + txData.toHex();
    ui->tcpMessage->append(txMessage);
    ui->btnDockingStation->setEnabled(false);
}
