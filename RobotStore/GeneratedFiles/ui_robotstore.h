/********************************************************************************
** Form generated from reading UI file 'robotstore.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROBOTSTORE_H
#define UI_ROBOTSTORE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RobotStoreClass
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *plcIP;
    QLabel *label_2;
    QLineEdit *plcPort;
    QPushButton *connectBtn;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLineEdit *serverIP;
    QLabel *label_3;
    QLineEdit *serverPort;
    QPushButton *listenBtn;
    QTextBrowser *tcpMessage;
    QPushButton *uiStartBtn;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RobotStoreClass)
    {
        if (RobotStoreClass->objectName().isEmpty())
            RobotStoreClass->setObjectName(QStringLiteral("RobotStoreClass"));
        RobotStoreClass->resize(1246, 873);
        centralWidget = new QWidget(RobotStoreClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(40, 30, 501, 441));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        plcIP = new QLineEdit(layoutWidget);
        plcIP->setObjectName(QStringLiteral("plcIP"));

        horizontalLayout->addWidget(plcIP);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        plcPort = new QLineEdit(layoutWidget);
        plcPort->setObjectName(QStringLiteral("plcPort"));

        horizontalLayout->addWidget(plcPort);

        connectBtn = new QPushButton(layoutWidget);
        connectBtn->setObjectName(QStringLiteral("connectBtn"));

        horizontalLayout->addWidget(connectBtn);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_2->addWidget(label_4);

        serverIP = new QLineEdit(layoutWidget);
        serverIP->setObjectName(QStringLiteral("serverIP"));

        horizontalLayout_2->addWidget(serverIP);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_2->addWidget(label_3);

        serverPort = new QLineEdit(layoutWidget);
        serverPort->setObjectName(QStringLiteral("serverPort"));

        horizontalLayout_2->addWidget(serverPort);

        listenBtn = new QPushButton(layoutWidget);
        listenBtn->setObjectName(QStringLiteral("listenBtn"));

        horizontalLayout_2->addWidget(listenBtn);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        tcpMessage = new QTextBrowser(layoutWidget);
        tcpMessage->setObjectName(QStringLiteral("tcpMessage"));

        gridLayout->addWidget(tcpMessage, 2, 0, 1, 1);

        uiStartBtn = new QPushButton(layoutWidget);
        uiStartBtn->setObjectName(QStringLiteral("uiStartBtn"));

        gridLayout->addWidget(uiStartBtn, 3, 0, 1, 1);

        RobotStoreClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RobotStoreClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1246, 21));
        RobotStoreClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RobotStoreClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        RobotStoreClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RobotStoreClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RobotStoreClass->setStatusBar(statusBar);

        retranslateUi(RobotStoreClass);

        QMetaObject::connectSlotsByName(RobotStoreClass);
    } // setupUi

    void retranslateUi(QMainWindow *RobotStoreClass)
    {
        RobotStoreClass->setWindowTitle(QApplication::translate("RobotStoreClass", "RobotStore", nullptr));
        label->setText(QApplication::translate("RobotStoreClass", "PLC IP : ", nullptr));
        label_2->setText(QApplication::translate("RobotStoreClass", "Port : ", nullptr));
        connectBtn->setText(QApplication::translate("RobotStoreClass", "Connect", nullptr));
        label_4->setText(QApplication::translate("RobotStoreClass", "Server IP : ", nullptr));
        label_3->setText(QApplication::translate("RobotStoreClass", "Port : ", nullptr));
        listenBtn->setText(QApplication::translate("RobotStoreClass", "Listen", nullptr));
        uiStartBtn->setText(QApplication::translate("RobotStoreClass", "Start UI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RobotStoreClass: public Ui_RobotStoreClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROBOTSTORE_H
