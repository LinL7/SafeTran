#include "CodeFactory.h"

CodeFactory::CodeFactory()
{
	isEncode = true;  //默认为true,但实际并不会以此为准
}

CodeFactory::~CodeFactory()
{
}

Codec* CodeFactory::createCodec()
{
	return nullptr;
}
