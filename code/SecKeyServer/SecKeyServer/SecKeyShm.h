#pragma once
#include "BaseShm.h"
#include "string";
#include<string.h>

using namespace std;

//�����ڴ��нڵ���Ϣ
struct ShmNodeInfo
{
	ShmNodeInfo() : status(0), number(0)
	{
		bzero(clientID, sizeof(clientID));
		bzero(serverID, sizeof(serverID));
		bzero(data, sizeof(data));
	}

	int number;				//�����ڴ���
	bool status;			//��ǰ��Կ�Ƿ����
	char serverID[20];		
	char clientID[20];
	char data[128];			//��Կ��Ϣ

};

class SecKeyShm:
	public BaseShm
{
public:
	//����ҵ��Ĺ����ڴ���
	SecKeyShm(int key);
	SecKeyShm(int key, int nodeNum);
	SecKeyShm(const char* pathname);
	SecKeyShm(const char* pathname, int nodeNum);
	~SecKeyShm();

	//��ʼ��,�ú���ֻ���𵽼������
	void initShm();

	//�����ڴ�д����Ϣ
	int writeNodeInfo(ShmNodeInfo* info);

	//����id��ȡ�ڵ���Ϣ--->û��Ҫ���ڵ���Ϣ��ֱ�Ӹ���ԿӦ�þ���
	ShmNodeInfo* readNodeInfo(const string serverID, const string clientID);

private:
	//���ڵ���
	int m_maxNodeNum; 
};

