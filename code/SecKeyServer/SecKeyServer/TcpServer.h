#pragma once
#include "TcpSocket.h"

class TcpServer
{
public:
	TcpServer();
	~TcpServer();
	
	//设置监听
	int setListen(unsigned short port);
	
	//接收连接，此处考虑超时连接
	TcpSocket* acceptConn(const unsigned int wait_second = 100);
	
	//关闭服务器
	void closeServer();

private:
	int m_lfd;
};

