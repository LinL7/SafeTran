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
	//��Ϊ�ǿͻ��ˣ�����Ҫ�ȴ����׽���
	m_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_fd < 0)
	{
		perror("socket: ");
		return -1;
	}
	//���÷�����
	setNonblock();
	//����������Ϣ����������
	struct sockaddr_in m_addr;
	m_addr.sin_addr.s_addr = inet_addr(ip.data());
	m_addr.sin_port = htons(port);
	m_addr.sin_family = AF_INET;
	//���������˷�������connect�᷵��-1����ȷ���������ж�-1��ͬʱ�����ж�errno�����ų������Ĵ���ԭ��
	ret = connect(m_fd, (const sockaddr*)&m_addr, sizeof(m_addr));
	if (ret < 0 && errno == EINPROGRESS)
	{
		//��ʱ��⣬���ӵĳ�ʱ���˳��ͬ
		ret = connTimeout();
	}

	//��������
	setBlock();
	return ret;
}

int TcpSocket::sendMsg(string data)
{
	int ret = -1;
	//׼���������ݣ����ͷ��������ճ��
	//׼��4���ֽڣ�������ݵ��ֽڳ�����Ϣ
	int len = data.size() + 4;
	char* netdata = (char*)malloc(len);
	if (netdata == nullptr)
	{
		printf("malloc error\n");
		return ret;
	}
	//ת���ֽ���
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
	//����ʱ���
	ret = recvTimeout();
	if (ret < 0)
	{
		return "";
	}

	//��ȡ����
	//�ȶ���ͷ,�����ͷ�ĳ�����Լ�����˵�,intռ4���ֽ�
	int netdatalen = 0;
	ret = readn(&netdatalen, 4);
	int n = ntohl(netdatalen);
	char* tmpBuf = (char*)malloc(n + 1); //n��string�ĳ��ȣ���char*���Դ�һ��'\0'
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
			printf("���ӳ�ʱ");
			return ret;
		}
		else
		{
			//��ʱҪ�ж��׽��ֵ�״̬���ж��Ƿ�����Ϊ����ʹ�׽��ֿ�д
			//��ȡ�׽���״̬
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
					//�׽���û�г�����д
					return 0;
				}
				else
				{
					printf("�׽��ֳ���\n");
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

	//��ʱʱ����0�����󷵻� -1
	do {
		ret = select(m_fd + 1, NULL, &wfds, NULL, &timeout);
	} while (ret < 0 && errno == EINTR); //����ʱ��ⱻ�ź��ж�ʱ������һ��
	if (ret < 0)
	{
		perror("select: ");
		return ret;
	}
	else if (ret == 0)
	{
		printf("д���ӳ�ʱ\n");
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

	//��ʱʱ����0�����󷵻� -1
	do {
		ret = select(m_fd + 1, &rfds, NULL, NULL, &timeout);
	} while (ret < 0 && errno == EINTR); //����ʱ��ⱻ�ź��ж�ʱ������һ��
	if (ret < 0)
	{
		perror("select: ");
		return ret;
	}
	else if (ret == 0)
	{
		printf("�����ӳ�ʱ\n");
		return ret;
	}
	return ret;
}

//��Ȼ�Ƕ�read��write�ĸ�д���������Ӧ����ԭ��������һ��
int TcpSocket::writen(const void* buf, int count)
{
	//��ͬ��read,��������count��buf�ĳ��ȣ����Ҫд��
	size_t nleft = count;	//�ѷ��ͳ���
	ssize_t nwritten = 0;	//ʣ�೤��
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
		//read��ȡһ������ֻ������ʣ�¶����ֽڼ���
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
			return count - nleft;  //�������˶���
		}
		bufp += nread; //����д��λ��
		nleft -= nread;
	}
	return count;
}

