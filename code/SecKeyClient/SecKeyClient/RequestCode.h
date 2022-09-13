#pragma once
#include "Codec.h"

using namespace std;

//������Ϣ���ݸ�ʽ
struct RequestInfo {
	int cmdType;
	string clientID ;
	string serverID;
	string sign;
	string data;
};

class RequestCode :
	public Codec
{
public:
	RequestCode();
	RequestCode(string str);
	RequestCode(struct RequestInfo* info);
	~RequestCode();

	//����
	string encStr();
	//����
	void* decStr();

private:
	string m_enstr;
	struct RequestInfo* m_info;
};

