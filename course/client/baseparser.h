#ifndef BASEPARSER_H_INCLUDED
#define BASEPARSER_H_INCLUDED
#include "common.h"

//整个工程解析部分基类
class BaseParser{
    protected:
	//主scheme        
	Scheme* mainScheme;
    public:
        BaseParser();
	//执行操作的函数        
	virtual void execute(vector<string> &result){};
        ~BaseParser(){};
};


#endif // BASEPARSER_H_INCLUDED
