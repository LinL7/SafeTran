#include <cstdio>
#include "TcpServer.h"
#include <iostream>
#include <pthread.h>
using namespace std;

void* callback(void* arg)
{
    TcpSocket* op = (TcpSocket*)arg;
    while (1)
    {
        string msg = op->recvMsg();
        if (msg.empty())
        {
            break;
        }
        cout << "recvMsg: " << msg << endl;
        string sendMsg = "";
        printf("请输入");
        cin >> sendMsg;
        op->sendMsg(sendMsg);
    }
    return nullptr;
}
int main()
{
    TcpServer* ser = new TcpServer;
    ser->setListen(6666);
    while (1)
    {
        cout << "开始accept" << endl;
        TcpSocket* op = ser->acceptConn();
        if (op)
        {
            pthread_t tid;
            pthread_create(&tid, NULL, callback, op);
            pthread_detach(tid);
        }
    }
    
    return 0;
}