#include <cstdio>
#include "TcpServer.h"
#include <iostream>
#include <pthread.h>
#include "RequestFactory.h"
#include "RespondFactory.h"
#include "CodeFactory.h"
#include "message.pb.h"

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

//测试通信
void testcomm()
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
}

//测试序列化
void testPro()
{
	// 数据编码
	RequestInfo info{ 1, "client", "server", "x00911", "hello, world" };
	CodeFactory* factory = new RequestFactory(&info);
	Codec* codec = factory->createCodec();
	cout << "开始编码" << endl;
	string str = codec->encStr();
	cout << "序列化数据: " << str << endl;
	delete factory;
	delete codec;

	// 数据解码
	factory = new RequestFactory(str);
	codec = factory->createCodec();
	cout << "开始解码" << endl;
	RequestInfo* r1 = (RequestInfo*)codec->decStr();
	cout << "cmdtype: " << r1->cmdType
		<< ", clinetID: " << r1->clientID
		<< ", serverID: " << r1->serverID
		<< ", sign: " << r1->sign
		<< ", data: " << r1->data << endl;
	delete factory;
	delete codec;

	cout << endl << "=========================" << endl;

	RespondInfo resinfo{ 1, 999, "luffy", "zoro", "change world 666 !" };
	factory = new RespondFactory(&resinfo);
	codec = factory->createCodec();
	str = codec->encStr();
	delete factory;
	delete codec;

	factory = new RespondFactory(str);
	codec = factory->createCodec();
	RespondInfo* r2 = (RespondInfo*)codec->decStr();
	cout << "status: " << r2->status
		<< ", seckeyID: " << r2->seckeyID
		<< ", clinetID: " << r2->clientID
		<< ", serverID: " << r2->serverID
		<< ", data: " << r2->data << endl;

	delete factory;
	delete codec;
}
int main()
{
    //testcomm();
    testPro();
    
    return 0;
}