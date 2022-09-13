#pragma once
#include "Codec.h"

using namespace std;
class CodeFactory
{
public:
	//工厂基类,提供创建对象的虚函数
	CodeFactory();
	~CodeFactory();

	virtual Codec* createCodec();
protected:
	bool isEncode;		//标识是否是编码
};

