#pragma once
#include "Codec.h"

using namespace std;

//请求消息内容格式
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

	//编码
	string encStr();
	//解码
	void* decStr();

private:
	string m_enstr;
	struct RequestInfo* m_info;
};

