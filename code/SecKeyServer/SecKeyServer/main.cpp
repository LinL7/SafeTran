#include <cstdio>
#include "TcpServer.h"
#include <iostream>
#include <pthread.h>
#include "RequestFactory.h"
#include "RespondFactory.h"
#include "CodeFactory.h"
#include "message.pb.h"
#include "SecKeyShm.h"

using namespace std;

//测试通信用
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

void writeShm();
void readShm();

//测试共享内存
void testshm() 
{
	//SecKeyShm shm();
	cout << "请输入要进行的操作: " << endl;
	cout << "	1. 写内存, 2. 读内存" << endl;
	int sel = 0;
	cin >> sel;
	switch (sel)
	{
	case 1:
		writeShm();
		break;
	case 2:
		readShm();
		break;
	default:
		break;
	}
	cout << "bye bye!!!" << endl;
}



int main()
{
    //testcomm();
    //testPro();
	testshm();
	int sel;
	cin >> sel;
    return 0;
}

//共享内存用
void writeShm()
{
	ShmNodeInfo info;
	//info.clientID = "kevin";
	//info.serverID = "Alice";
	info.number = 100;
	//info.data = "我是基督山伯爵...";
	info.status = 1;
	strcpy(info.clientID, "kevin");
	strcpy(info.serverID, "Alice");
	strcpy(info.data, "我是基督山伯爵...");
	//info.seckeyID = 100;
	//info.status = 1;
	SecKeyShm shm("/home", 10);
	shm.initShm();
	int ret = shm.writeNodeInfo(&info);
	if (ret == -1)
	{
		printf("写入失败");
	}

	//info.clientID = "Bob";
	//info.serverID = "Jack";
	info.number = 666;
	//info.data = "基督山伯爵就是 - 爱德蒙·唐代斯!!!";
	info.status = 1;
	strcpy(info.clientID, "Bob");
	strcpy(info.serverID, "Jack");
	strcpy(info.data, "基督山伯爵就是 - 爱德蒙·唐代斯!!!");
	//info.seckeyID = 666;
	//info.status = 1;
	shm.writeNodeInfo(&info);
}

//共享内存用
void readShm()
{
	char tmp[128];
	strcpy(tmp, "Bob");
	SecKeyShm shm("/home", 10);
	ShmNodeInfo* info = shm.readNodeInfo(tmp, "Jack");
	if (info = nullptr)
	{
		printf("没读到");
		return;
	}
	cout << "ClientID: " << info->clientID << endl;
	cout << "ServerID: " << info->serverID << endl;
	cout << "SecKey: " << info->data << endl;
	cout << "seckeyID: " << info->number << endl;
	cout << "status: " << info->status << endl;
	// shm.delShm();
}