#include "RespondCode.h"
#include "message.pb.h"

RespondCode::RespondCode()
{
}

RespondCode::RespondCode(string str)
{
	m_encstr = str;
}

RespondCode::RespondCode(RespondInfo* info)
{
	m_info = info;
}

string RespondCode::encStr()
{
	string out;
	RespondMsg resMsg;
	
	resMsg.set_status(m_info->status);
	resMsg.set_seckeyid(m_info->seckeyID);
	resMsg.set_clientid(m_info->clientID);
	resMsg.set_serverid(m_info->serverID);
	resMsg.set_data(m_info->data);
	resMsg.SerializeToString(&out);

	return out;
}

void* RespondCode::decStr()
{
	RespondMsg* resMsg = new RespondMsg();
	RespondInfo* info = new RespondInfo();
	resMsg->ParseFromString(m_encstr);
	info->status = resMsg->status();
	info->seckeyID = resMsg->seckeyid();
	info->clientID = resMsg->clientid();
	info->serverID = resMsg->serverid();
	info->data = resMsg->data();

	return info;
}
