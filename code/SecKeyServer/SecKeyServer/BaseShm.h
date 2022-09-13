#pragma once
class BaseShm
{
public:

	~BaseShm();
	//ͨ���ؼ��ִ������ߴ򿪹����ڴ�,
	BaseShm(int key, int size);
	BaseShm(int key);
	//ͨ��·����������򿪹����ڴ�
	BaseShm(const char* pathname, int size);
	BaseShm(const char* pathname);

	//���������ڴ浽��ǰ����
	void* shmAssoc();
	//ȡ�������ڴ�
	int shmCan(const void* shmaddr);
	//ɾ�������ڴ�(���)
	int shmDel(int cmd, struct shmid_ds* buf);

private:
	int m_shmid;	//�����ڴ�id

protected:
	void* m_addr;	//�����ڴ��׵�ַ
};

