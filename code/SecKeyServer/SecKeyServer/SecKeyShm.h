#pragma once
#include "BaseShm.h"
#include "string";
#include<string.h>

using namespace std;

//共享内存中节点信息
struct ShmNodeInfo
{
	ShmNodeInfo() : status(0), number(0)
	{
		bzero(clientID, sizeof(clientID));
		bzero(serverID, sizeof(serverID));
		bzero(data, sizeof(data));
	}

	int number;				//共享内存编号
	bool status;			//当前秘钥是否可用
	char serverID[20];		
	char clientID[20];
	char data[128];			//秘钥信息

};

class SecKeyShm:
	public BaseShm
{
public:
	//面向业务的共享内存类
	SecKeyShm(int key);
	SecKeyShm(int key, int nodeNum);
	SecKeyShm(const char* pathname);
	SecKeyShm(const char* pathname, int nodeNum);
	~SecKeyShm();

	//初始化,该函数只是起到检测作用
	void initShm();

	//向共享内存写入信息
	int writeNodeInfo(ShmNodeInfo* info);

	//根据id获取节点信息--->没必要给节点信息，直接给秘钥应该就行
	ShmNodeInfo* readNodeInfo(const string serverID, const string clientID);

private:
	//最大节点数
	int m_maxNodeNum; 
};

