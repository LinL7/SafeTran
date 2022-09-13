#include "BaseShm.h"
#include <sys/shm.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>

BaseShm::~BaseShm()
{
}

BaseShm::BaseShm(int key, int size)
{
	m_shmid = shmget(key, size, IPC_CREAT | 0664);
	if (m_shmid == -1)
	{
		printf("创建共享内存出错\n");
	}
}

BaseShm::BaseShm(int key)
{
	//这个函数没有指定size，使用的情况应该是打开
	m_shmid = shmget(key, 0, 0);
	if (m_shmid == -1)
	{
		printf("打开共享内存出错\n");
	}
}

BaseShm::BaseShm(const char* pathname, int size)
{
	key_t key = ftok(pathname, 1);	//第二个参数可以随意设定
	if (key == -1)
	{
		printf("通过路径名创建key失败");
	}
	m_shmid = shmget(key, size, IPC_CREAT | 0664);
	if (m_shmid == -1)
	{
		printf("创建共享内存出错\n");
	}
}


BaseShm::BaseShm(const char* pathname)
{
	key_t key = ftok(pathname, 1);	//第二个参数可以随意设定
	if (key == -1)
	{
		printf("通过路径名创建key失败");
	}
	m_shmid = shmget(key, 0, 0);
	if (m_shmid == -1)
	{
		printf("创建共享内存出错\n");
	}
}

void* BaseShm::shmAssoc()
{
	//让系统来分配位置
	m_addr = shmat(m_shmid, NULL, 0);
	if (m_addr == (void*)-1)
	{
		printf("关联共享内存出错\n");
		return (void*)-1;
	}
	return m_addr;
}

int BaseShm::shmCan(const void* shmaddr)
{
	int ret = shmdt(shmaddr);
	if (ret == -1)
	{
		printf("取消关联共享内存失败");
		return -1;
	}
	return ret;
}

int BaseShm::shmDel(int cmd, shmid_ds* buf)
{
	int ret = shmctl(m_shmid, IPC_RMID, NULL);
	if (ret == -1)
	{
		printf("标记删除失败");
	}
	return ret;
}
