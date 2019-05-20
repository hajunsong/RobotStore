#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpclient.h"

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
    void connectBtnSlot();
    void onConnectServer();
    void readMessage();
    void positionBtnSlot();
    void btnDockingStationSlot();

private:
    Ui::MainWindow *ui;
    TcpClient *client;
    bool connectState, guestState;
};

#endif // MAINWINDOW_H
