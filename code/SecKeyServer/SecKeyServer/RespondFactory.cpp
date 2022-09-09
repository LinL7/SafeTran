#include "RespondFactory.h"

RespondFactory::RespondFactory()
{
}

RespondFactory::RespondFactory(string str)
{
	m_str = str;
	isEncode = false;
}

RespondFactory::RespondFactory(RespondInfo* info)
{
	m_info = info;
	isEncode = true;
}

Codec* RespondFactory::createCodec()
{
	if (isEncode)
		return new RespondCode(m_info);
	else
		return new RespondCode(m_str);
}
