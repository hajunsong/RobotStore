#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpserver.h"
#include "tcpclient.h"

#include "qclicklabel.h"
#include "slidingstackedwidget.h"

#include <QDebug>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPicture>
#include <QTimer>
#include <QMouseEvent>
#include <QEvent>

#include <iostream>
using namespace std;

const int sizeColor = 6, sizePattern = 7;
const QString imageHeader = "../icons/";
const QString imageIcon[sizeColor*sizePattern] = { "a_1@2x","a_2@2x","a_3@2x","a_4@2x","a_5@2x","a_6@2x","a_7@2x",
                                                   "b_1@2x","b_2@2x","b_3@2x","b_4@2x","b_5@2x","b_6@2x","b_7@2x",
                                                   "c_1@2x","c_2@2x","c_3@2x","c_4@2x","c_5@2x","c_6@2x","c_7@2x",
                                                   "d_1@2x","d_2@2x","d_3@2x","d_4@2x","d_5@2x","d_6@2x","d_7@2x",
                                                   "e_1@2x","e_2@2x","e_3@2x","e_4@2x","e_5@2x","e_6@2x","e_7@2x",
                                                   "f_1@2x","f_2@2x","f_3@2x","f_4@2x","f_5@2x","f_6@2x","f_7@2x" };
const QString startIcon[2] = { "start_P_hangul", "start_R_hangul" };
const QString colorIcon[sizeColor] = { "n1", "n2", "n3", "n4", "n5", "n6" };
const QString patternIcon[sizePattern] = { "g1", "g2", "g3", "g4", "g5", "g6", "g7" };
const QString orderIcon[3] = { "choose_hangul", "order_hangul", "order_1_hangul" };
const QString clerkIcon = "clerk_hangul";
const QString thankIcon = "thank_hangul";
const QString waitIcon = "wait_hangul";
enum { startPage, selectPage, itemPage, thankPage };

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void listenBtnSlot();
    void readMessageFromMR();
    void connectedClient();
    void disconnected();

    void connectBtnSlot();
    void onConnectPLC();
    void readMessageFromPLC();

    void uiStartBtnSlot();

    void timeout();

    void startBtnPressedSlot();
    void startBtnReleasedSlot();
    void resetBtnSlot();
    void colorBtnPressedSlot();
    void patternBtnPressedSlot();
    void combineItem();
    void orderBtnPressedSlot();
    void orderBtnReleasedSlot();
    void delayTimeout();

    void singleCbSlot();

private:
    Ui::MainWindow *ui;
    TcpServer *MR;
    TcpClient *PLC;
    bool connectStateMR, connectStatePLC, robotPositionState, PLCReady;
    QTimer *timer;

    SlidingStackedWidget *mainUI;
    QWidget *pageWidget;
    // 1 Page
    QClickLabel *startBtn;
    QPushButton *resetBtn;
    // 2 Page
    QClickLabel *colorBtn[6], *patternBtn[7], *orderBtn;
    QWidget *bottomLayout;
    QLabel *clerkLabel;
    QTimer *delayTimer;
    // 3 Page
    QLabel *itemLabel, *waitLabel;
    int colorIndex, patternIndex;
    // 4 Page

    QLabel *thankLabel;

    bool move_flag;
    bool start_flag;
    bool single_flag;

    void selectPageInit();
    void checkAllConnectState();
    void clickLabelDrawImage(QClickLabel *clickLabel, QString imagePath, double scale);
    void labelDrawImage(QLabel *label, QString imagePath, double scale);
};

#endif // MAINWINDOW_H
