#include <cstdio>
#include "TcpSocket.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    TcpSocket* op = new TcpSocket;
	//服务器一定要先启动，不然会出错
    op->connectToHost("127.0.0.1", 6666);
    cout << "连接成功 " << endl;

	// 通信
	while (1)
	{
		string sendmsg = "hello server ...";
		cout << "发送数据" << sendmsg << endl;
		op->sendMsg(sendmsg);
		// 接收数据
		int recvLen = -1;
		string recvMsg = op->recvMsg();
		cout << "recvmsg: " << recvMsg << endl;
		sleep(1);
	}
	return 0;
}