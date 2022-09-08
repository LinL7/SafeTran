#include "TcpSocket.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>

TcpSocket::TcpSocket()
{

}

TcpSocket::TcpSocket(int fd)
{
	m_fd = fd;
}

TcpSocket::~TcpSocket()
{
}

int TcpSocket::connectToHost(string ip, unsigned short port)
{
	int ret = -1;
	//因为是客户端，所以要先创建套接字
	m_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_fd < 0)
	{
		perror("socket: ");
		return -1;
	}
	//设置非阻塞
	setNonblock();
	//设置连接信息，进行连接
	struct sockaddr_in m_addr;
	m_addr.sin_addr.s_addr = inet_addr(ip.data());
	m_addr.sin_port = htons(port);
	m_addr.sin_family = AF_INET;
	//这里设置了非阻塞，connect会返回-1，正确的做法是判断-1的同时，再判断errno，以排除其他的错误原因
	ret = connect(m_fd, (const sockaddr*)&m_addr, sizeof(m_addr));
	if (ret < 0 && errno == EINPROGRESS)
	{
		//超时检测，连接的超时检测顺序不同
		ret = connTimeout();
	}

	//设置阻塞
	setBlock();
	return ret;
}

int TcpSocket::sendMsg(string data)
{
	int ret = -1;
	//准备发送数据，添加头部，避免粘包
	//准备4个字节，添加数据的字节长度信息
	int len = data.size() + 4;
	char* netdata = (char*)malloc(len);
	if (netdata == nullptr)
	{
		printf("malloc error\n");
		return ret;
	}
	//转换字节序
	int datalen = htonl(data.size());
	memcpy(netdata, &datalen, 4);
	memcpy(netdata + 4, data.data(), data.size());
	ret = sendTimeout();
	if (ret <= 0)
	{
		return -1;
	}
	ret = writen(netdata, len);
	if (ret < len)
	{
		printf("send error\n");
		return -1;
	}
	
	return ret;
}

string TcpSocket::recvMsg()
{
	int ret = -1;
	//读超时检测
	ret = recvTimeout();
	if (ret < 0)
	{
		return "";
	}

	//读取数据
	//先读报头,这个报头的长度是约定好了的,int占4个字节
	int netdatalen = 0;
	ret = readn(&netdatalen, 4);
	int n = ntohl(netdatalen);
	char* tmpBuf = (char*)malloc(n + 1); //n是string的长度，而char*会自带一个'\0'
	if (tmpBuf == nullptr)
	{
		perror("malloc: ");
		return nullptr;
	}
	ret = readn(tmpBuf, n);
	if (ret == -1)
	{
		printf("readn error\n");
		return string();
	}
	else if (ret < n)
	{
		printf("readn num error\n");
		return string();
	}
	tmpBuf[n] = '\0';
	string data = string(tmpBuf);
	return data;
}

void TcpSocket::setNonblock()
{
	int oldopt = fcntl(m_fd, F_GETFL);
	int newopt = oldopt | O_NONBLOCK;
	fcntl(m_fd, F_SETFL, newopt);
}

void TcpSocket::setBlock()
{
	int oldopt = fcntl(m_fd, F_GETFL);
	int newopt = oldopt & ~O_NONBLOCK;
	fcntl(m_fd, F_SETFL, newopt);
}

int TcpSocket::connTimeout(const unsigned int wait_second)
{
	int ret = -1;
	if (wait_second > 0)
	{
		fd_set wfds;
		timeval timeout;
	
		FD_ZERO(&wfds);
		FD_SET(m_fd, &wfds);
		timeout.tv_sec = wait_second;
		timeout.tv_usec = 0;

		do {
			ret = select(m_fd + 1, NULL, &wfds, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret < 0)
		{
			perror("select: ");
			return ret;
		}
		else if (ret == 0)
		{
			printf("连接超时");
			return ret;
		}
		else
		{
			//此时要判断套接字的状态来判断是否是因为错误使套接字可写
			//获取套接字状态
			int err = -1;
			socklen_t len = sizeof(err);
			int status = getsockopt(m_fd, SOL_SOCKET, SO_ERROR, &err, &len);
			if(status < 0)
			{ 
				perror("getsockopt: ");
				return -1;
			}
			else
			{
				if (err == 0)
				{
					//套接字没有出错，可写
					return 0;
				}
				else
				{
					printf("套接字出错\n");
					return -1;
				}
			}
		}
		return ret;
	}
	
	


	return ret;
}

int TcpSocket::sendTimeout(const unsigned int wait_second)
{
	int ret = -1;
	if (wait_second <= 0)
		return ret;
	fd_set wfds;
	timeval timeout;

	FD_ZERO(&wfds);
	FD_SET(m_fd, &wfds);
	timeout.tv_sec = wait_second;
	timeout.tv_usec = 0;

	//超时时返回0，错误返回 -1
	do {
		ret = select(m_fd + 1, NULL, &wfds, NULL, &timeout);
	} while (ret < 0 && errno == EINTR); //当超时检测被信号中断时，再来一次
	if (ret < 0)
	{
		perror("select: ");
		return ret;
	}
	else if (ret == 0)
	{
		printf("写连接超时\n");
		return ret;
	}
	return ret;
}

int TcpSocket::recvTimeout(const unsigned int wait_second)
{
	int ret = -1;
	if (wait_second <= 0)
		return ret;
	fd_set rfds;
	timeval timeout;

	FD_ZERO(&rfds);
	FD_SET(m_fd, &rfds);
	timeout.tv_sec = wait_second;
	timeout.tv_usec = 0;

	//超时时返回0，错误返回 -1
	do {
		ret = select(m_fd + 1, &rfds, NULL, NULL, &timeout);
	} while (ret < 0 && errno == EINTR); //当超时检测被信号中断时，再来一次
	if (ret < 0)
	{
		perror("select: ");
		return ret;
	}
	else if (ret == 0)
	{
		printf("读连接超时\n");
		return ret;
	}
	return ret;
}

//既然是对read和write的改写，传入参数应该与原函数保持一致
int TcpSocket::writen(const void* buf, int count)
{
	//不同与read,传进来的count是buf的长度，因此要写完
	size_t nleft = count;	//已发送长度
	ssize_t nwritten = 0;	//剩余长度
	int tmp = 0;
	char* bufp = (char*)buf;
	
	while (nleft > 0)
	{
		//
		if ((nwritten = write(m_fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			return -1;
		}
		else if (nwritten == 0)
		{
			continue;
		}
		bufp += nwritten;
		nleft -= nwritten;
	}
	return count;
}


int TcpSocket::readn(void* buf, int count)
{
	size_t nleft = count;
	ssize_t nread = 0;
	char* bufp = (char*)buf;
	while (nleft > 0)
	{
		//read读取一个流，只需标记下剩下多少字节即可
		if ((nread = read(m_fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			return -1;
		}
		else if (nread == 0)
		{
			return count - nleft;  //传出读了多少
		}
		bufp += nread; //更换写入位置
		nleft -= nread;
	}
	return count;
}

