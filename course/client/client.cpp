#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <cassert>
#include "common.h"
#include "../include/client.h"
#include "selectparser.h"
#include "insertparser.h"
#include "baseparser.h"
using namespace std;

/*map<string, vector<string> > table2name;
map<string, vector<string> > table2type;
map<string, vector<string> > table2pkey;*/
vector<string> result;
Scheme totScheme;
char totBuffer[MAX_MEM];


//建立表
//table 表名
//column 列名
//type 每一列的数据类型
//key  主键
void create(const string& table, const vector<string>& column,
	const vector<string>& type, const vector<string>& key)
{
	//FILE *fout;
	//fout = fopen(((string) "data/" + table).c_str(), "a+");
	//fclose(fout);
	/*table2name[table] = column;
	table2type[table] = type;
	table2pkey[table] = key;*/
	Table* tmpTable = new Table;
	//get the name;
	tmpTable->tableName = table;
	tmpTable->storeUnit = new HybridStorage(table);
	//get the columns' name
	tmpTable->columeName = column;
    Colume*tmp;
    //tmp.cond.clear();
    //建立表内的列的对应，类型和顺序
    tmpTable->typeList.clear();
    for(int i = 0;i < type.size();i ++){
        tmp = new Colume;
        tmp->preKey = 0;
        if (type[i] == "INTEGER")
            tmp->type = 0;
        else
            tmp->type = 1;
        tmpTable->typeList.push_back(tmp->type);
        tmp->order = i;
        tmpTable->columeType[column[i]] = tmp;
    }
    //获得主键
    for(int i = 0;i < key.size();i ++)
        tmpTable->columeType[key[i]]->preKey = 1;

    //将表插入到scheme中
    totScheme.tableList.push_back(tmpTable);

    /*
    printf("creating:table=%s\n",tmpTable->tableName.c_str());
    for(int i = 0;i < tmpTable->columeName.size();i ++)
        printf("%s,%d \n",tmpTable->columeName[i].c_str(),tmpTable->columeType[column[i]]->type);
    */
}

//TODO 训练函数
void train(const vector<string>& query, const vector<double>& weight)
{
	vector<string, double> tmpVec;
	tmpVec.clear();
	for(int i = 0;i < totScheme.tableList.size();i ++){
        tmpVec[totScheme.tableList[i]->tableName] = 0;
    vector<string> token;

    //计算权值
    for(int i =0;i < query.size();i ++){
        token.clear();
        tokenize(query[i].c_str(),token);
        int j;
        for(j = 0;j < token.size();j ++)
            if (token[i] == "FROM")
                break;
        for(j++;j < token.size();j ++){
            if (token[i] == "," || token[i] == ";")
                continue;
            if (token[i] == "WHERE")
                break;
            tmpVec[token[i]] += weight[i];

        }
    }
    //冒泡排序表
    int tmpLen = totScheme.tableList.size();
    Table* tmpTable;
    for(int i = 0;i < tmpLen;i ++)
        for(int j = 0;j < tmpLen - 1 - i;j ++)
            if (tmpVec[totScheme ->tableList[j]->tableName] < tmpVec[totScheme ->tableList[j + 1]->tableName]){
                tmpTable = totScheme -> tableList[j];
                totScheme -> tableList[j] = totScheme -> tableList[j + 1];
                totScheme -> tableList[j + 1] = tmpTable ;
            }
}

//读入初始数据
void load(const string& table, const vector<string>& row)
{
    vector<string> token;
    for(int i = 0;i < totScheme->tableList.size();i ++)
        if (table == totScheme->tableList[i]->tableName){
            totScheme->tableList[i]->storeUnit->writeStart();
            //token.clear();
            for(int j = 0;j < row.size();j ++){
                token.clear();
                split_csv(row[j].c_str(),token);
                totScheme->tableList[i]->storeUnit->writeData(token,totScheme->tableList[i]->typeList);
            }
            totScheme->tableList[i]->storeUnit->writeFinish();
            break;
        }
}

//TODO 建立索引之类的
void preprocess(){
    char* tmpP = totBuffer;
    int reMem = MAX_MEM;
    int tmpM;
    //迁移到内存
    for(int i = 0;i < totScheme->tableList.size();i ++){
        tmpM = totScheme->tableList[i]->storeUnit->moveToMem(tmpP,reMem);
        if (tmpM == 0)
            break;
        tmpP += (reMem - tmpM);
        reMem = tmpM;
    }
}

//执行SQL语句
void execute(const string& sql)
{
    BaseParser* totParser;
    //判断是什么类型的语句SELECT 或 INSERT
    if (sql[0] == 'S')
        totParser = new SelectParser(sql,&totScheme);
    else
        totParser = new InsertParser(sql,&totScheme);
    totParser ->execute(result);

}

//获得结果
int next(char *row)
{
	if (result.size() == 0)
		return (0);
	strcpy(row, result.back().c_str());
	result.pop_back();

	/*
	 * This is for debug only. You should avoid unnecessary output
	 * in your submission, which will hurt the performance.
	 */
	printf("%s\n", row);
	return (1);
}

void close()
{
	// I have nothing to do.
}


