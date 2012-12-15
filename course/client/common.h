#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include<string>
#include<cstring>
#include<vector>
#include<map>
#include<cstdio>
#include<cstdlib>
#include"hybridstorage.h"

using namespace std;

//用来进行join的判断，两个Join的condition指向同一个JoinCon,哪一个先访问，就改变firstCol，下一个就要通过strKey来判断
struct JoinCon{
    string strKey;
    string firstCol;
};

//Where中的条件语句，kind： 0=  1< 2> 3 join
struct Condition{
    int kind;
    string strConst;
    JoinCon* joinCon;
};

//表示列，顺序，类型，是否为主键
struct Colume{
    int order;
    int type;
    bool preKey;
    //vector<Condition> cond;

};

//表 表名  列名  列名到列类型的映射  
struct Table{
    string tableName;
    vector<string> columeName;
    vector<int> typeList;
    map<string,Colume*> columeType;
    //FILE tarFile;
    HybridStorage* storeUnit;

};


struct Scheme{
    vector<Table*> tableList;

};

typedef unsigned char uchar;

const int STD_BUFF = 100;
const int MAX_MEM = 500000000;
const int BLOCK_LEN = 30000;
#endif // COMMON_H_INCLUDED
