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

//建立表
//table 表名
//column 列名
//type 每一列的数据类型
//key  主键
void create(const string& table, const vector<string>& column,
	const vector<string>& type, const vector<string>& key)
{
	/*table2name[table] = column;
	table2type[table] = type;
	table2pkey[table] = key;*/
	Table* tmpTable = new Table;
	//get the name;
	tmpTable->tableName = table;
	//get the columns' name
	tmpTable->columeName = column;
    Colume*tmp;
    //tmp.cond.clear();
    //建立表内的列的对应，类型和顺序
    for(int i = 0;i < type.size();i ++){
        tmp = new Colume;
        tmp->preKey = 0;
        if (type[i] == "INTEGER")
            tmp->type = 0;
        else
            tmp->type = 1;
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
	// I am too clever; I don't need it.
}

//读入初始数据
void load(const string& table, const vector<string>& row)
{
	FILE *fout;
	int i;

	fout = fopen(((string) "data/" + table).c_str(), "w");
	assert(fout != NULL);

	for (i = 0; i < row.size(); i++)
		fprintf(fout, "%s\n", row[i].c_str());

	fclose(fout);
}

//TODO 建立索引之类的
void preprocess()
{
	// I am too clever; I don't need it.
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


