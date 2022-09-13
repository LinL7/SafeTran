#include "RequestCode.h"
#include "message.pb.h"

RequestCode::RequestCode()
{
}

RequestCode::RequestCode(string str)
{
	m_enstr = str;
}

RequestCode::RequestCode(RequestInfo* info)
{
	m_info = info;
}

RequestCode::~RequestCode()
{
}

string RequestCode::encStr()
{
	string out;

	RequestMsg reqMsg;
	reqMsg.set_cmdtype(m_info->cmdType);
	reqMsg.set_clientid(m_info->clientID);
	reqMsg.set_serverid(m_info->serverID);
	reqMsg.set_sign(m_info->sign);
	reqMsg.set_data(m_info->data);
	reqMsg.SerializeToString(&out);

	return out;
}

void* RequestCode::decStr()
{
	RequestMsg* reqMsg = new RequestMsg();
	RequestInfo* info;
	info = new RequestInfo();
	reqMsg->ParseFromString(m_enstr);
	info->sign = reqMsg->sign();
	info->cmdType = reqMsg->cmdtype();
	
	info->clientID = reqMsg->clientid();
	info->serverID = reqMsg->serverid();
	
	info->data = reqMsg->data();

	return info;
}
