#include "RequestFactory.h"

RequestFactory::RequestFactory()
{
}

RequestFactory::RequestFactory(string str)
{
	isEncode = false;
	m_str = str;
}

RequestFactory::RequestFactory(RequestInfo* info)
{
	isEncode = true;
	m_info = info;
}

Codec* RequestFactory::createCodec()
{
	if (isEncode)
		return new RequestCode(m_info);
	else
		return new RequestCode(m_str);
}
