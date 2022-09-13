#pragma once
#include "Codec.h"

using namespace std;

struct RespondInfo{
	int status;
	int seckeyID;
	string clientID;
	string serverID;
	string data;		//�������ظ������ﱣ����ǶԳƼ��ܵ���Կ
};

class RespondCode:
	public Codec
{
public:
	RespondCode();
	RespondCode(string str);
	RespondCode(struct RespondInfo* info);

	string encStr();
	void* decStr();

private:
	string m_encstr;
	struct RespondInfo* m_info;

};

