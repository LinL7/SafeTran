#pragma once
#include <string>

class Codec
{
public:
	Codec();
	~Codec();
	
	//±àÂë
	virtual std::string encStr();
	virtual void* decStr();
};

