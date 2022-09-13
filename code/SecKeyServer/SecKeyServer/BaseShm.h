#pragma once
class BaseShm
{
public:

	~BaseShm();
	//通过关键字创建或者打开共享内存,
	BaseShm(int key, int size);
	BaseShm(int key);
	//通过路径名创建或打开共享内存
	BaseShm(const char* pathname, int size);
	BaseShm(const char* pathname);

	//关联共享内存到当前进程
	void* shmAssoc();
	//取消关联内存
	int shmCan(const void* shmaddr);
	//删除共享内存(标记)
	int shmDel(int cmd, struct shmid_ds* buf);

private:
	int m_shmid;	//共享内存id

protected:
	void* m_addr;	//共享内存首地址
};

