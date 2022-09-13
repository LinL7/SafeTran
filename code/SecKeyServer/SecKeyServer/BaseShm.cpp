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
		printf("���������ڴ����\n");
	}
}

BaseShm::BaseShm(int key)
{
	//�������û��ָ��size��ʹ�õ����Ӧ���Ǵ�
	m_shmid = shmget(key, 0, 0);
	if (m_shmid == -1)
	{
		printf("�򿪹����ڴ����\n");
	}
}

BaseShm::BaseShm(const char* pathname, int size)
{
	key_t key = ftok(pathname, 1);	//�ڶ����������������趨
	if (key == -1)
	{
		printf("ͨ��·��������keyʧ��");
	}
	m_shmid = shmget(key, size, IPC_CREAT | 0664);
	if (m_shmid == -1)
	{
		printf("���������ڴ����\n");
	}
}


BaseShm::BaseShm(const char* pathname)
{
	key_t key = ftok(pathname, 1);	//�ڶ����������������趨
	if (key == -1)
	{
		printf("ͨ��·��������keyʧ��");
	}
	m_shmid = shmget(key, 0, 0);
	if (m_shmid == -1)
	{
		printf("���������ڴ����\n");
	}
}

void* BaseShm::shmAssoc()
{
	//��ϵͳ������λ��
	m_addr = shmat(m_shmid, NULL, 0);
	if (m_addr == (void*)-1)
	{
		printf("���������ڴ����\n");
		return (void*)-1;
	}
	return m_addr;
}

int BaseShm::shmCan(const void* shmaddr)
{
	int ret = shmdt(shmaddr);
	if (ret == -1)
	{
		printf("ȡ�����������ڴ�ʧ��");
		return -1;
	}
	return ret;
}

int BaseShm::shmDel(int cmd, shmid_ds* buf)
{
	int ret = shmctl(m_shmid, IPC_RMID, NULL);
	if (ret == -1)
	{
		printf("���ɾ��ʧ��");
	}
	return ret;
}
