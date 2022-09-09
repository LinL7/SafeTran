#pragma once
#include "CodeFactory.h"
#include "RespondCode.h"

class RespondFactory:
	public CodeFactory
{
public:
	RespondFactory();
	RespondFactory(string str);
	RespondFactory(RespondInfo* info);

	Codec* createCodec();
private:
	string m_str;
	RespondInfo* m_info;

};

