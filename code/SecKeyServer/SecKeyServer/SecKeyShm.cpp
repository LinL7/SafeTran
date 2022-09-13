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
	//当shm非空时，将其初始化，重新全部置为0
	if (m_addr != nullptr)
	{
		memset(m_addr, '\0', m_maxNodeNum * sizeof(ShmNodeInfo));
	}
}

int SecKeyShm::writeNodeInfo(ShmNodeInfo* info)
{
	//根据id信息，遍历已有该节点，有则覆盖，无则再遍历，找到第一个为0的地点，写入
	ShmNodeInfo* startAddr = (ShmNodeInfo*)shmAssoc();	//标志共享内存起始位置，赋类型以便遍历
	ShmNodeInfo* tmp2;
	for (int i = 0; i < m_maxNodeNum; i++)
	{
		tmp2 = startAddr + i;
		if (tmp2->clientID == info->clientID && tmp2->serverID == info->serverID)
		{
			//已存在节点，更新信息
			memcpy(tmp2, info, sizeof(info));
			return 0;			//更新成功，返回0
		}
	}
	ShmNodeInfo tmp;
	memset(&tmp, 0, sizeof(tmp));

	for (int i = 0; i < m_maxNodeNum; i++)
	{
		tmp2 = startAddr + i;
		if (memcmp(&tmp, tmp2, sizeof(ShmNodeInfo)) == 0)
		{
			cout << "写入新节点 " << i << endl;+
			//不能用memcpy()拷贝string
			memcpy(startAddr + i, info, sizeof(info));
			return 0;			//更新成功，返回0
		}
	}

	return -1;	//没有填入信息，失败返回-1
}

ShmNodeInfo* SecKeyShm::readNodeInfo(const string serverID, const string clientID)
{
	//逐个循环遍历是否存在即可
	ShmNodeInfo* startAddr = (ShmNodeInfo*)shmAssoc();	//标志共享内存起始位置，赋类型以便遍历
	for (int i = 0; i < m_maxNodeNum; i++)
	{
		if ((startAddr + i)->clientID == clientID && (startAddr + i)->serverID == serverID)
		{
			//已存在节点，更新信息
			ShmNodeInfo* ret = new ShmNodeInfo();		//应该返回节点的拷贝，免得节点信息被直接篡改
			strcmp(ret->clientID,(startAddr + i)->clientID);
			strcmp(ret->serverID, (startAddr + i)->serverID);
			ret->number = (startAddr + i)->number;
			ret->status = (startAddr + i)->status;
			strcmp(ret->data, (startAddr + i)->data);
			return ret;			//更新成功，返回0
		}
	}
	return nullptr;	//循环一遍不存在
}
