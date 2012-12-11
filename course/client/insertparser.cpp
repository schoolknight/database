#include "insertparser.h"
//构造函数，解析过程
InsertParser::InsertParser(const string& sql,Scheme* tar){
    mainScheme = tar;
    vector<string> token;
    token.clear();
    //把语句切分开
    tokenize(sql.c_str(), token);
    //查找目标表
    for(int i = 0;i < mainScheme->tableList.size();i ++)
        if (mainScheme->tableList[i]->tableName == token[2]){
            tarTable = mainScheme->tableList[i];
            break;
        }
    //获得目标表列的数量
    colLen = tarTable->columeName.size();
    valueList.clear();
    //获得插入的值
    for(int i = 4;i < token.size();i ++){
        if (token[i] == "(" || token[i] == "," || token[i] == ")")
            continue;
        valueList.push_back(token[i]);
    }
}
//实际执行插入，即写入文件
void InsertParser::execute(vector<string> &result){
   	FILE *fout;
	int i;
	fout = fopen(((string) "data/" + tarTable->tableName).c_str(), "a+");
	assert(fout != NULL);
   	string str = "";
	for (i = 0; i < valueList.size(); i++){
		str += valueList[i];
		if ((i + 1) % colLen == 0){
            fprintf(fout, "%s\n", str.c_str());
            str = "";
            fflush(fout);
		}
		else
            str += ",";
	}
	fclose(fout);
}
