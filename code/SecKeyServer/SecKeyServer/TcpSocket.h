#pragma once
#include <string>

using namespace std;

class TcpSocket
{
public:
	TcpSocket();
	TcpSocket(int fd);
	~TcpSocket();

	//���ӵ����������ṩ���ͻ���
	int connectToHost(string ip, unsigned short port);
	//��������
	int sendMsg(string data);
	//��������
	string recvMsg();
	//�Ͽ�����
	int closeConn();
	
	

private:
	//���÷�����
	void setNonblock();
	//��������
	void setBlock();
	//���ӳ�ʱ����
	int connTimeout(const unsigned int wait_second = 100);
	//���ͳ�ʱ���
	int sendTimeout(const unsigned int wait_second = 100);
	//���ճ�ʱ���
	int recvTimeout(const unsigned int wait_second = 100);
	//����ָ���ֽ�����
	int writen(const void* buf, int count);
	//����ָ���ֽ�����
	int readn(void *buf, int count);
private:
	int m_fd;
};

