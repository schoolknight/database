#ifndef INSERTPARSER_H_INCLUDED
#define INSERTPARSER_H_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include<algorithm>

#include "../include/client.h"
#include "tokenize.h"
#include "split_csv.h"
#include "common.h"
#include "baseparser.h"

using namespace std;

//解释执行INSERT语句
class InsertParser : public BaseParser{
    private:
	//插入的目标表        
	Table* tarTable;
	//插入的value值        
	vector<string> valueList;
	//目标表的列的数量        
	int colLen;
    public:
	//构造函数，即解析过程        
	InsertParser(const string& sql,Scheme* tar);
	//执行语句        
	void execute(vector<string> &result);

};

#endif // INSERTPARSER_H_INCLUDED
