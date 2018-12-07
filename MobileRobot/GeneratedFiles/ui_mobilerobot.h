/********************************************************************************
** Form generated from reading UI file 'mobilerobot.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOBILEROBOT_H
#define UI_MOBILEROBOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
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

class Ui_MobileRobotClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *ipAddress;
    QPushButton *connectBtn;
    QCheckBox *guestCbox;
    QPushButton *positionBtn;
    QTextBrowser *tcpMessage;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MobileRobotClass)
    {
        if (MobileRobotClass->objectName().isEmpty())
            MobileRobotClass->setObjectName(QStringLiteral("MobileRobotClass"));
        MobileRobotClass->resize(535, 481);
        centralWidget = new QWidget(MobileRobotClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        ipAddress = new QLineEdit(centralWidget);
        ipAddress->setObjectName(QStringLiteral("ipAddress"));

        horizontalLayout->addWidget(ipAddress);

        connectBtn = new QPushButton(centralWidget);
        connectBtn->setObjectName(QStringLiteral("connectBtn"));

        horizontalLayout->addWidget(connectBtn);

        guestCbox = new QCheckBox(centralWidget);
        guestCbox->setObjectName(QStringLiteral("guestCbox"));

        horizontalLayout->addWidget(guestCbox);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        positionBtn = new QPushButton(centralWidget);
        positionBtn->setObjectName(QStringLiteral("positionBtn"));

        gridLayout->addWidget(positionBtn, 1, 0, 1, 1);

        tcpMessage = new QTextBrowser(centralWidget);
        tcpMessage->setObjectName(QStringLiteral("tcpMessage"));

        gridLayout->addWidget(tcpMessage, 2, 0, 1, 1);

        MobileRobotClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MobileRobotClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 535, 21));
        MobileRobotClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MobileRobotClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MobileRobotClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MobileRobotClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MobileRobotClass->setStatusBar(statusBar);

        retranslateUi(MobileRobotClass);

        QMetaObject::connectSlotsByName(MobileRobotClass);
    } // setupUi

    void retranslateUi(QMainWindow *MobileRobotClass)
    {
        MobileRobotClass->setWindowTitle(QApplication::translate("MobileRobotClass", "MobileRobot", nullptr));
        label->setText(QApplication::translate("MobileRobotClass", "IP Address : ", nullptr));
        connectBtn->setText(QApplication::translate("MobileRobotClass", "Connect", nullptr));
        guestCbox->setText(QApplication::translate("MobileRobotClass", "Guest Came In", nullptr));
        positionBtn->setText(QApplication::translate("MobileRobotClass", "Mobile robot arrives in front of guest", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MobileRobotClass: public Ui_MobileRobotClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOBILEROBOT_H
