#pragma once
#include "Codec.h"

using namespace std;
class CodeFactory
{
public:
	//��������,�ṩ����������麯��
	CodeFactory();
	~CodeFactory();

	virtual Codec* createCodec();
protected:
	bool isEncode;		//��ʶ�Ƿ��Ǳ���
};

