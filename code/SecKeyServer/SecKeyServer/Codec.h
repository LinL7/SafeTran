#pragma once
#include <string>

class Codec
{
public:
	Codec();
	~Codec();
	
	//����
	virtual std::string encStr();
	virtual void* decStr();
};

