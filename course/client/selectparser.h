#ifndef SELECTPARSER_H_INCLUDED
#define SELECTPARSER_H_INCLUDED


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


class SelectParser : public BaseParser{
    private:
        //whole scheme
        //Scheme* mainScheme;
        //column orderg
	//查找的列在查找队列中的位置对应
	map<string,int> ansList;
        //select condition
	//列的条件的对应
	map<string,vector <Condition> > condList;
        //
	//各个表中那些列是被请求的
	vector<int> comList[50];
	//各个表中那些列是有条件的
	vector<int> condTableList[50];
	//那些表是被请求的
	vector<int> questTable;
	//保留答案
	vector<string> row;
	//判断是否满足条件   key 当前的键值  rule对应列所有的条件   colName当前列的列名
	bool checkToken(const string &key,vector<Condition> &rule,const string &colName);

	//递归执行select操作  step 深度   result 结果
	void selectExec(int step,vector<string> &result);


    public:
        SelectParser(const string& sql,Scheme* tar);
        void execute(vector<string> &result);



};

#endif // SELECTPARSER_H_INCLUDED
