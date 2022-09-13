#include "SecKeyShm.h"
#include <string.h>
#include <iostream>

using namespace std;

SecKeyShm::SecKeyShm(int key, int nodeNum):BaseShm(key, nodeNum * sizeof(ShmNodeInfo)),m_maxNodeNum(nodeNum)
{
}

SecKeyShm::SecKeyShm(int key) : BaseShm(key)
{
}

SecKeyShm::SecKeyShm(const char* pathname) : BaseShm(pathname)
{
}

SecKeyShm::SecKeyShm(const char* pathname, int nodeNum) : BaseShm(pathname, nodeNum * sizeof(ShmNodeInfo)),m_maxNodeNum(nodeNum)
{
}


SecKeyShm::~SecKeyShm()
{

}

void SecKeyShm::initShm()
{
	//��shm�ǿ�ʱ�������ʼ��������ȫ����Ϊ0
	if (m_addr != nullptr)
	{
		memset(m_addr, '\0', m_maxNodeNum * sizeof(ShmNodeInfo));
	}
}

int SecKeyShm::writeNodeInfo(ShmNodeInfo* info)
{
	//����id��Ϣ���������иýڵ㣬���򸲸ǣ������ٱ������ҵ���һ��Ϊ0�ĵص㣬д��
	ShmNodeInfo* startAddr = (ShmNodeInfo*)shmAssoc();	//��־�����ڴ���ʼλ�ã��������Ա����
	ShmNodeInfo* tmp2;
	for (int i = 0; i < m_maxNodeNum; i++)
	{
		tmp2 = startAddr + i;
		if (tmp2->clientID == info->clientID && tmp2->serverID == info->serverID)
		{
			//�Ѵ��ڽڵ㣬������Ϣ
			memcpy(tmp2, info, sizeof(info));
			return 0;			//���³ɹ�������0
		}
	}
	ShmNodeInfo tmp;
	memset(&tmp, 0, sizeof(tmp));

	for (int i = 0; i < m_maxNodeNum; i++)
	{
		tmp2 = startAddr + i;
		if (memcmp(&tmp, tmp2, sizeof(ShmNodeInfo)) == 0)
		{
			cout << "д���½ڵ� " << i << endl;+
			//������memcpy()����string
			memcpy(startAddr + i, info, sizeof(info));
			return 0;			//���³ɹ�������0
		}
	}

	return -1;	//û��������Ϣ��ʧ�ܷ���-1
}

ShmNodeInfo* SecKeyShm::readNodeInfo(const string serverID, const string clientID)
{
	//���ѭ�������Ƿ���ڼ���
	ShmNodeInfo* startAddr = (ShmNodeInfo*)shmAssoc();	//��־�����ڴ���ʼλ�ã��������Ա����
	for (int i = 0; i < m_maxNodeNum; i++)
	{
		if ((startAddr + i)->clientID == clientID && (startAddr + i)->serverID == serverID)
		{
			//�Ѵ��ڽڵ㣬������Ϣ
			ShmNodeInfo* ret = new ShmNodeInfo();		//Ӧ�÷��ؽڵ�Ŀ�������ýڵ���Ϣ��ֱ�Ӵ۸�
			strcmp(ret->clientID,(startAddr + i)->clientID);
			strcmp(ret->serverID, (startAddr + i)->serverID);
			ret->number = (startAddr + i)->number;
			ret->status = (startAddr + i)->status;
			strcmp(ret->data, (startAddr + i)->data);
			return ret;			//���³ɹ�������0
		}
	}
	return nullptr;	//ѭ��һ�鲻����
}
