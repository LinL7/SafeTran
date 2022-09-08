#pragma once
#include <string>

using namespace std;

class TcpSocket
{
public:
	TcpSocket();
	TcpSocket(int fd);
	~TcpSocket();

	//连接到服务器，提供给客户端
	int connectToHost(string ip, unsigned short port);
	//发送数据
	int sendMsg(string data);
	//接收数据
	string recvMsg();
	//断开连接
	int closeConn();
	
	

private:
	//设置非阻塞
	void setNonblock();
	//设置阻塞
	void setBlock();
	//连接超时处理
	int connTimeout(const unsigned int wait_second = 100);
	//发送超时检测
	int sendTimeout(const unsigned int wait_second = 100);
	//接收超时检测
	int recvTimeout(const unsigned int wait_second = 100);
	//发送指定字节数据
	int writen(const void* buf, int count);
	//接收指定字节数据
	int readn(void *buf, int count);
private:
	int m_fd;
};

