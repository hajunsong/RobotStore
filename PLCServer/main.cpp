#include <QCoreApplication>
#include "tcpserver.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    char ip[256];
    sprintf(ip, "127.0.0.1");
//    quint16 port;
//    cout << "Please enter IP Address" << endl;
//    cin >> ip; // 127.0.0.1 "192.168.173.5"
//    cout << "Please enter Port number" << endl;
//    cin >> port; // 9999

    TcpServer *server = new TcpServer();
    server->setting(ip, 9999);
    server->startServer();

    return a.exec();
}
