#pragma once
#include "TcpSocket.h"

class TcpServer
{
public:
	TcpServer();
	~TcpServer();
	
	//���ü���
	int setListen(unsigned short port);
	
	//�������ӣ��˴����ǳ�ʱ����
	TcpSocket* acceptConn(const unsigned int wait_second = 100);
	
	//�رշ�����
	void closeServer();

private:
	int m_lfd;
};

