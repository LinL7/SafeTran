#include "CodeFactory.h"

CodeFactory::CodeFactory()
{
	isEncode = true;  //Ĭ��Ϊtrue,��ʵ�ʲ������Դ�Ϊ׼
}

CodeFactory::~CodeFactory()
{
}

Codec* CodeFactory::createCodec()
{
	return nullptr;
}
