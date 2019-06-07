#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(1920, 1080);
    this->setWindowTitle("Robot Store");

    ReadSettings();

    connect(ui->listenBtn, SIGNAL(clicked()), this, SLOT(listenBtnSlot()));
    connect(ui->connectBtn, SIGNAL(clicked()), this, SLOT(connectBtnSlot()));

    MR = new TcpServer();
    PLC = new TcpClient();

    connectStateMR = false;
    connectStatePLC = false;
    PLCReady = false;

    connect(PLC->socket, SIGNAL(connected()), this, SLOT(onConnectPLC()));
    connect(PLC->socket, SIGNAL(readyRead()), this, SLOT(readMessageFromPLC()));

    connect(ui->uiStartBtn, SIGNAL(clicked()), this, SLOT(uiStartBtnSlot()));
    ui->uiStartBtn->setDisabled(true);

    timer = new QTimer(this);
    timer->setInterval(3000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

    mainUI = new SlidingStackedWidget(this);
    mainUI->hide();
    mainUI->setGeometry(this->rect());

    uint pages = 4;
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
        colorBtn[i]->setGeometry(static_cast<int>(pageWidth*(0.04 + i * 0.15)), static_cast<int>(pageHeight / 3 * 0.3), pageWidth / 6, pageHeight / 3);
        connect(colorBtn[i], SIGNAL(mousePressed()), this, SLOT(colorBtnPressedSlot()));
        connect(colorBtn[i], SIGNAL(mouseReleased()), this, SLOT(combineItem()));
    }

    for (int i = 0; i < sizePattern; i++) {
        patternBtn[i] = new QClickLabel(&pageWidget[selectPage]);
        patternBtn[i]->setObjectName("Pattern " + QString::number(i + 1));
//        patternBtn[i]->setGeometry(static_cast<int>(pageWidth*(0.035 + i * 0.13)), static_cast<int>(pageHeight / 3 * 1.2), pageWidth / 7, pageHeight / 3);
        patternBtn[i]->setGeometry(static_cast<int>(pageWidth*(0.04 + i * 0.15)), static_cast<int>(pageHeight / 3 * 1.2), pageWidth / 6, pageHeight / 3);
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
    itemLabel->setGeometry(static_cast<int>(pageWidth*0.2), static_cast<int>(pageHeight*0.2), static_cast<int>(pageWidth*0.6), static_cast<int>(pageHeight*0.6));
    itemLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

    waitLabel = new QLabel(&pageWidget[itemPage]);
    waitLabel->setGeometry(static_cast<int>(pageWidth*0.2), static_cast<int>(pageHeight*0.8), static_cast<int>(pageWidth*0.6), static_cast<int>(pageHeight*0.2));
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
    auto_flag = false;
    full_auto_flag = false;

    connect(ui->cbSingleTest, SIGNAL(clicked()), this, SLOT(singleCbSlot()));
    connect(ui->cbAuto, SIGNAL(clicked()), this, SLOT(cbAutoSlot()));

    QDateTime *date = new QDateTime();
    _mkdir("../logging");
    fileName = "../logging/" + date->currentDateTime().toString("yyyy-MM-dd") + ".txt";
    logger = new Logger(this, fileName);
    logger->write("------- Application Startup --------");

    tcpTimer = new QTimer(this);
    tcpTimer->setInterval(1000);
    connect(tcpTimer, SIGNAL(timeout()), this, SLOT(tcpTimeout()));

    noGuestTimer = new QTimer(this);
    noGuestTimer->setInterval(300000);
    connect(noGuestTimer, SIGNAL(timeout()), this, SLOT(noGuestTimeout()));
    no_guest_flag = false;

    stopInputTimer = new QTimer(this);
    stopInputTimer->setInterval(60000);
    connect(stopInputTimer, SIGNAL(timeout()), this, SLOT(stopInputTimeout()));
}

MainWindow::~MainWindow()
{
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

    logger->write("------- Application Finished --------");
    delete logger;

    delete ui;
}

void MainWindow::ReadSettings()
{
    QSettings settings("Robot Store", "UI Project");
    //     restoreGeometry(settings.value("geometry").toByteArray());
    //     restoreState(settings.value("windowState").toByteArray());
    QString PLC_Server_IP = settings.value("PLC_Server_IP").toString();
    QString PLC_Server_PORT = settings.value("PLC_Server_PORT").toString();
    QString Server_IP = settings.value("Server_IP").toString();
    QString Server_PORT = settings.value("Server_PORT").toString();
    ui->plcIP->setText(PLC_Server_IP);
    ui->plcPort->setText(PLC_Server_PORT);
    ui->serverIP->setText(Server_IP);
    ui->serverPort->setText(Server_PORT);
}

void MainWindow::WriteSettings()
{
     QSettings settings("Robot Store", "UI Project");
     settings.setValue("PLC_Server_IP", ui->plcIP->text());
     settings.setValue("PLC_Server_PORT", ui->plcPort->text());
     settings.setValue("Server_IP", ui->serverIP->text());
     settings.setValue("Server_PORT", ui->serverPort->text());
//     settings.setValue("geometry", saveGeometry());
//     settings.setValue("windowState", saveState());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (connectStatePLC){
        PLC->socket->close();
    }
    if (connectStateMR){
        MR->socket->close();
    }
    WriteSettings();
    QMainWindow::closeEvent(event);
}

void MainWindow::selectPageInit() {
    for (int i = 0; i < sizeColor; i++) clickLabelDrawImage(colorBtn[i], imageHeader + colorIcon[i], 0.35);
    for (int i = 0; i < sizePattern; i++) clickLabelDrawImage(patternBtn[i], imageHeader + patternIcon[i], 0.35);

    orderBtn->setGeometry(bottomLayout->width() / 6, 0, bottomLayout->width() / 3 * 2, bottomLayout->height() / 5 * 4);
    clickLabelDrawImage(orderBtn, imageHeader + orderIcon[0], 0.2);

    colorIndex = -1;
    patternIndex = -1;
}

void MainWindow::listenBtnSlot() {
    MR->setIpAddress(ui->serverIP->text());
    MR->setPort(ui->serverPort->text().toUShort());
    MR->startServer();

    connect(MR, SIGNAL(connectedClient()), this, SLOT(connectedClient()));

    colorIndex = -1;
    patternIndex = -1;

    ui->listenBtn->setDisabled(true);
}

void MainWindow::connectBtnSlot() {
    if (connectStatePLC) {
        PLC->socket->close();
        ui->tcpMessage->append("PLC Server Disconnect ...");
        logger->write("PLC Server Disconnect ...");
        connectStatePLC = false;
        ui->connectBtn->setText("Connect");
        ui->uiStartBtn->setDisabled(true);
    }
    else {
        PLC->setIpAddress(ui->plcIP->text());
        PLC->setPort(ui->plcPort->text().toUShort());
        emit PLC->connectToServer();
    }
}

void MainWindow::onConnectPLC() {
    ui->tcpMessage->append("PLC Server Connect complete ...");
    logger->write("PLC Server Connect complete ...");
    connectStatePLC = true;
    ui->connectBtn->setText("Disconnect");
}

void MainWindow::readMessageFromPLC() {
    QString rxMessage;
    QByteArray rxData = PLC->socket->readAll();

    rxMessage = "Receive Data(From PLC) : " + rxData.toHex();
    ui->tcpMessage->append(rxMessage);
    logger->write(rxMessage);

    char *ch = new char[static_cast<uint>(rxData.length())];
    for (int j = 0; j < rxData.length(); j++) {
        ch[j] = rxData.at(j);
    }
    qDebug() << "Receive Data(From PLC) : " + rxData.toHex();

    if (PLCReady) {
        if (ch[0] == 0x03 && ch[1] == 0x06) {
            if (ch[2] == 0x50)
                //if ((ch[2] & 0b00010000) == 0b00010000)
            {
                qDebug() << "Manipulator operating";
                logger->write("Manipulator operating");
            }
            else if (ch[2] == 0x60  && move_flag)
                //else if ((ch[2] & 0b00100000) == 0b00100000 && move_flag)
            {
                qDebug() << "Manipulator operation complete";
                logger->write("Manipulator operation complete");
                mainUI->slideInIdx(thankPage, mainUI->TOP2BOTTOM);

                if (!single_flag) {
                    QByteArray txDataMR;
                    txDataMR.append(QByteArray::fromRawData("\x02\x05", 2));
                    txDataMR.append(QByteArray::fromRawData("\x00", 1));
                    txDataMR.append(QByteArray::fromRawData("\x0D\x05", 2));
                    if (connectStateMR){
                        MR->socket->write(txDataMR);
                        qDebug() << "Transmit Data(To MR) : " + txDataMR.toHex();
                        logger->write("Transmit Data(To MR) : " + txDataMR.toHex());
                    }
                    else{
                        qDebug() << "Disconnected MR";
                        logger->write("Disconnected MR");
                    }
                }
                else{
                    timer->start();
                }

                move_flag = false;
            }
        }
    }
    else {
        if (ch[0] == 0x03 && ch[1] == 0x06) {
//            if ((ch[2] & 0b01000000) == 0b01000000)
                //if (ch[2] == 0x60)
            if (ch[2] == 0x00 || ch[2] == 0x60 || ch[2] == 0x40)
            {
                qDebug() << "Manipulator ready!!";
                ui->tcpMessage->append("Manipulator ready!!");
                logger->write("Manipulator ready!!");
                PLCReady = true;
                checkAllConnectState();
            }
        }
    }
    delete[] ch;
}

void MainWindow::uiStartBtnSlot() {
    ui->centralWidget->hide();
    mainUI->setHidden(false);
    selectPageInit();
    noGuestTimer->start();
}

void MainWindow::timeout() {
    timer->stop();
    qDebug() << "Mobile robot arrives in front of docking station";
    logger->write("Mobile robot arrives in front of docking station");

    mainUI->slideInIdx(startPage, mainUI->TOP2BOTTOM);
    selectPageInit();
    if (!single_flag) robotPositionState = false;
}

void MainWindow::startBtnPressedSlot() {
    clickLabelDrawImage(startBtn, imageHeader + startIcon[1], 0.3);
    noGuestTimer->stop();
}

void MainWindow::startBtnReleasedSlot() {
    clerkLabel->show();

    if (full_auto_flag){
        connect(mainUI, &SlidingStackedWidget::animationFinished, this, &MainWindow::pageChangedSlot);

        clerkLabel->hide();

        clickLabelDrawImage(startBtn, imageHeader + startIcon[0], 0.3);
        mainUI->slideInIdx(selectPage, mainUI->BOTTOM2TOP);
    }
    else{
        if (!single_flag) {
            QByteArray txDataMR;
            txDataMR.append(QByteArray::fromRawData("\x02\x05", 2));
            txDataMR.append(QByteArray::fromRawData("\x01", 1));
            txDataMR.append(QByteArray::fromRawData("\x0D\x05", 2));
            MR->socket->write(txDataMR);
            qDebug() << "Transmit Data(To MR) : " + txDataMR.toHex();
            logger->write("Transmit Data(To MR) : " + txDataMR.toHex());
        }
        else {
            clerkLabel->hide();

            clickLabelDrawImage(startBtn, imageHeader + startIcon[0], 0.3);
            mainUI->slideInIdx(selectPage, mainUI->BOTTOM2TOP);
        }
    }
}

void MainWindow::resetBtnSlot() {
    ui->centralWidget->setHidden(false);
    mainUI->hide();
    selectPageInit();
}

void MainWindow::colorBtnPressedSlot() {
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

void MainWindow::patternBtnPressedSlot() {
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

void MainWindow::orderBtnPressedSlot() {
    if (colorIndex >= 0 && patternIndex >= 0) {
        clickLabelDrawImage(orderBtn, imageHeader + orderIcon[2], 0.2);
    }
}

void MainWindow::orderBtnReleasedSlot() {
    stopInputTimer->stop();
    if (colorIndex >= 0 && patternIndex >= 0) {
        clickLabelDrawImage(orderBtn, imageHeader + orderIcon[1], 0.2);

        mainUI->slideInIdx(itemPage, mainUI->BOTTOM2TOP);
        int itemIndex = colorIndex * sizePattern + patternIndex;
        labelDrawImage(itemLabel, imageHeader + imageIcon[itemIndex], 4);

        QByteArray txData;
        txData.append(QByteArray::fromRawData("\x02\x05", 2));
        txData.append(static_cast<char>(2*(itemIndex + 1) - 1));
        txData.append(QByteArray::fromRawData("\x01", 1));
        txData.append(QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00", 8));
        txData.append(QByteArray::fromRawData("\x0D\x05", 2));
        PLC->socket->write(txData);
        PLC->socket->flush();
        qDebug() << "Transmit Data(To PLC) : " + txData.toHex();
        logger->write("Transmit Data(To PLC) : " + txData.toHex());

        if (!single_flag) {
            QByteArray txDataMR;
            txDataMR.append(QByteArray::fromRawData("\x02\x05", 2));
            txDataMR.append(QByteArray::fromRawData("\x03", 1));
            txDataMR.append(QByteArray::fromRawData("\x0D\x05", 2));

            MR->socket->write(txDataMR);
            qDebug() << "Transmit Data(To MR) : " + txDataMR.toHex();
            logger->write("Transmit Data(To MR) : " + txDataMR.toHex());
        }
        delayTimer->start();
    }
}

void MainWindow::checkAllConnectState() {
    if (connectStateMR && PLCReady) {
        ui->uiStartBtn->setEnabled(true);
        uiStartBtnSlot();
    }
    if (single_flag) {
        if (PLCReady) {
            uiStartBtnSlot();
            robotPositionState = true;
        }
    }
}

void MainWindow::combineItem() {
    if (colorIndex >= 0 && patternIndex >= 0) {
        orderBtn->setGeometry(bottomLayout->width() / 3, 0, bottomLayout->width() / 3, bottomLayout->height() / 5 * 4);
        clickLabelDrawImage(orderBtn, imageHeader + orderIcon[1], 0.2);
    }
}

void MainWindow::clickLabelDrawImage(QClickLabel *clickLabel, QString imagePath, double scale) {
    QImage *image = new QImage();
    QPixmap *buffer = new QPixmap();
    if (image->load(imagePath)) {
        *buffer = QPixmap::fromImage(*image);
        *buffer = buffer->scaled(static_cast<int>(image->width()*scale), static_cast<int>(image->height()*scale));
    }
    clickLabel->setPixmap(*buffer);
    clickLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
    delete image;
    delete buffer;
}

void MainWindow::labelDrawImage(QLabel *label, QString imagePath, double scale) {
    QImage *image = new QImage();
    QPixmap *buffer = new QPixmap();
    if (image->load(imagePath)) {
        *buffer = QPixmap::fromImage(*image);
        *buffer = buffer->scaled(static_cast<int>(image->width()*scale), static_cast<int>(image->height()*scale));
    }
    label->setPixmap(*buffer);
    label->setAlignment(Qt::AlignmentFlag::AlignCenter);
    delete image;
    delete buffer;
}

void MainWindow::connectedClient() {
    connectStateMR = true;
    connect(MR->socket, SIGNAL(readyRead()), this, SLOT(readMessageFromMR()), Qt::DirectConnection);
    connect(MR->socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);
    ui->tcpMessage->append("Mobile Robot Connected");
    logger->write("Mobile Robot Connected");
    checkAllConnectState();
    tcpTimer->start();
    if (mainUI->currentIndex() != startPage){
        mainUI->slideInIdx(startPage, mainUI->TOP2BOTTOM);
    }
    robotPositionState = false;
    clerkLabel->hide();
    selectPageInit();
    clickLabelDrawImage(startBtn, imageHeader + startIcon[0], 0.3);
}

void MainWindow::readMessageFromMR() {
    QByteArray rxData = MR->socket->readAll();
    QString rxMessage;

    qDebug() << "Receive Data(From MR) : " + rxData.toHex();
    ui->tcpMessage->append("Receive Data(From MR) : " + rxData.toHex());
    logger->write("Receive Data(From MR) : " + rxData.toHex());
    QChar *ch = new QChar[static_cast<uint>(rxData.length())];
    for (int j = 0; j < rxData.length(); j++) {
        ch[j] = rxData.at(j);
    }
    if (ch[0] == 0x02 && ch[1] == 0x05) {
        if (ch[2] == 0x02) {
            qDebug() << "Mobile robot arrives in front of guest";
            logger->write("Mobile robot arrives in front of guest");
            robotPositionState = true;
            clerkLabel->hide();

            clickLabelDrawImage(startBtn, imageHeader + startIcon[0], 0.3);
            mainUI->slideInIdx(selectPage, mainUI->BOTTOM2TOP);
            stopInputTimer->start();

            QByteArray txData;
            txData.append(QByteArray::fromRawData("\x02\x05", 2));
            txData.append(QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10));
            txData.append(QByteArray::fromRawData("\x0D\x05", 2));

            PLC->socket->write(txData);
            PLC->socket->flush();
            qDebug() << "Transmit Data(To PLC) : " + txData.toHex();
            logger->write("Transmit Data(To PLC) : " + txData.toHex());

            if (auto_flag){
                QThread::sleep(1);
                colorIndex = 2;
                patternIndex = 2;

                QThread::sleep(1);
                orderBtnReleasedSlot();
            }
        }
        else if(ch[2] == 0x05){
            qDebug() << "Mobile robot arrives in front of docking station";
            logger->write("Mobile robot arrives in front of docking station");

            mainUI->slideInIdx(startPage, mainUI->TOP2BOTTOM);
            selectPageInit();
            if (!single_flag) robotPositionState = false;

            if (auto_flag){
                QThread::sleep(20);
                startBtnPressedSlot();
                QThread::sleep(1);
                startBtnReleasedSlot();
            }

            noGuestTimer->start();

            no_guest_flag = false;
            startBtn->setEnabled(true);
        }
    }
}

void MainWindow::disconnected() {
    connectStateMR = false;
    qDebug() << QString::number(MR->socket->socketDescriptor()) + " Disconnected";
    ui->tcpMessage->append(QString::number(MR->socket->socketDescriptor()) + " Disconnected");
    logger->write(QString::number(MR->socket->socketDescriptor()) + " Disconnected");
    MR->socket->deleteLater();
    MR->socket->close();
    tcpTimer->stop();
}

void MainWindow::delayTimeout() {
    QByteArray txData;
    txData.append(QByteArray::fromRawData("\x02\x05", 2));
    txData.append(QByteArray::fromRawData("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10));
    txData.append(QByteArray::fromRawData("\x0D\x05", 2));

    PLC->socket->write(txData);
    PLC->socket->flush();
    qDebug() << "Transmit Data(To PLC) : " + txData.toHex();
    logger->write("Transmit Data(To PLC) : " + txData.toHex());

    delayTimer->stop();

    move_flag = true;
}

void MainWindow::singleCbSlot() {
    single_flag = ui->cbSingleTest->isChecked();
    logger->write("PLC single test " + QString::number(single_flag));
    if (single_flag && auto_flag) full_auto_flag = true;
    if (connectStatePLC){
        PLCReady = true;
        checkAllConnectState();
    }
}

void MainWindow::cbAutoSlot(){
    auto_flag = ui->cbAuto->isChecked();
    logger->write("UI auto test " + QString::number(auto_flag));
    if (single_flag && auto_flag) full_auto_flag = true;
}

void MainWindow::tcpTimeout(){
    tcpData.clear();
    tcpData.append(QByteArray::fromRawData("\x02\x05", 2));
    tcpData.append(QByteArray::fromRawData("\x06", 1));
    tcpData.append(QByteArray::fromRawData("\x0D\x05", 2));
    MR->socket->write(tcpData);

    qDebug() << "Transmit Data(To MR) : " + tcpData.toHex();
    logger->write("Transmit Data(To MR) : " + tcpData.toHex());
}

void MainWindow::noGuestTimeout(){
    qDebug() << "No guest";
    logger->write("No guest");
    QByteArray txDataMR;
    txDataMR.append(QByteArray::fromRawData("\x02\x05", 2));
    txDataMR.append(QByteArray::fromRawData("\x08", 1));
    txDataMR.append(QByteArray::fromRawData("\x0D\x05", 2));

    MR->socket->write(txDataMR);
    qDebug() << "Transmit Data(To MR) : " + txDataMR.toHex();
    logger->write("Transmit Data(To MR) : " + txDataMR.toHex());
    noGuestTimer->stop();
    no_guest_flag = true;
    startBtn->setEnabled(false);
}

void MainWindow::stopInputTimeout(){
    qDebug() << "Leaved guest";
    logger->write("Leaved guest");
    QByteArray txDataMR;
    txDataMR.append(QByteArray::fromRawData("\x02\x05", 2));
    txDataMR.append(QByteArray::fromRawData("\x07", 1));
    txDataMR.append(QByteArray::fromRawData("\x0D\x05", 2));

    MR->socket->write(txDataMR);
    qDebug() << "Transmit Data(To MR) : " + txDataMR.toHex();
    logger->write("Transmit Data(To MR) : " + txDataMR.toHex());
    stopInputTimer->stop();

    if (mainUI->currentIndex() != startPage){
        mainUI->slideInIdx(startPage, mainUI->TOP2BOTTOM);
    }
    robotPositionState = false;
    clerkLabel->hide();
    selectPageInit();
    clickLabelDrawImage(startBtn, imageHeader + startIcon[0], 0.3);
}

void MainWindow::pageChangedSlot(){
//    qDebug() << mainUI->currentIndex();
    int page = mainUI->currentIndex();
    switch(page){
    case startPage:
        QThread::sleep(1);
        mainUI->slideInIdx(selectPage, mainUI->BOTTOM2TOP);
        break;
    case selectPage:
        colorIndex = 2;
        patternIndex = 2;

        QThread::sleep(1);
        orderBtnReleasedSlot();
        break;
    case itemPage:
        break;
    case thankPage:
        break;
    default:
        break;
    }
}
