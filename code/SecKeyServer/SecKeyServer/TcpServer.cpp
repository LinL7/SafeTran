#include "TcpServer.h"
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <unistd.h>

TcpServer::TcpServer()
{
}

TcpServer::~TcpServer()
{
}

int TcpServer::setListen(unsigned short port)
{
	int ret = -1;
	//�����׽���
	m_lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_lfd < 0)
	{
		perror("socket: ");
		return -1;
	}
	//���ö˿ڸ���,�����ڰ�֮ǰ����ʹ����ͬ�˿ڵĶ�Ҫ���ö˿ڸ���
	int opt = 1;
	ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEPORT, (const void*)&opt, sizeof(opt));
	if (ret < 0)
	{
		perror("SOL_REUSEPORT: ");
		return ret;
	}
	//�����׽�����Ϣ
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);		//������ת�����ֽ���
	my_addr.sin_addr.s_addr = INADDR_ANY;		//���󶨵������ܹ�������Ϣ��ip
	int len = sizeof(my_addr);
	//��
	ret = bind(m_lfd, (const struct sockaddr*)&my_addr, len);
	if (ret < 0)
	{
		perror("bind: ");
		return ret;
	}
	//����
	ret = listen(m_lfd, 128);
	if (ret < 0)
	{
		perror("listen: ");
		return ret;
	}
	return ret;
}

TcpSocket* TcpServer::acceptConn(const unsigned int wait_second)
{
	int ret = -1;
	fd_set rfds;
	timeval timeout;

	FD_ZERO(&rfds);
	FD_SET(m_lfd, &rfds);
	timeout.tv_sec = wait_second;
	timeout.tv_usec = 0;

	//��ʱʱ����0�����󷵻� -1
	do {
		ret = select(m_lfd + 1, &rfds, NULL, NULL, &timeout);
	} while (ret < 0 && errno == EINTR); //����ʱ��ⱻ�ź��ж�ʱ������һ��
	if (ret < 0)
	{
		perror("select: ");
		return nullptr;
	}
	else if (ret == 0)
	{
		printf("�������ӳ�ʱ\n");
		return nullptr;
	}
	struct sockaddr_in laddr;
	memset(&laddr, 0, sizeof(laddr));
	socklen_t len = sizeof(laddr);
	int lfd = accept(m_lfd, (sockaddr*)&laddr, &len);
	if (lfd < 0)
	{
		perror("accept: ");
		return nullptr;
	}
	else
	{
		printf("�ͻ������ӳɹ���ip: %s, port: %s\n", std::to_string(ntohl(laddr.sin_addr.s_addr)), std::to_string(laddr.sin_port));
		return new TcpSocket(lfd);
	}
	return nullptr;
}

void TcpServer::closeServer()
{
	close(m_lfd);
}
