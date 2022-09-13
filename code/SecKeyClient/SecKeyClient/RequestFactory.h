#pragma once
#include "CodeFactory.h"
#include "RequestCode.h"

class RequestFactory:
	public CodeFactory
{
public:
	RequestFactory();
	RequestFactory(string str);
	RequestFactory(RequestInfo* info);

	Codec* createCodec();
private:
	string m_str;
	RequestInfo* m_info;
};

