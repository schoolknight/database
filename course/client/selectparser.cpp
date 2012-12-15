#include "selectparser.h"

//构造函数，解析语句
SelectParser::SelectParser(const string& sql,Scheme* tar){
    //super();
    mainScheme = tar;
    vector<string> token;
    token.clear();
    tokenize(sql.c_str(), token);
    vector<string> tmpList;
    tmpList.clear();
    questTable.clear();
    int i;
    for (i = 0; i < token.size(); i++) {
		if (token[i] == "SELECT" || token[i] == ",")
			continue;
		if (token[i] == "FROM")
			break;
		//输出哪些列
		tmpList.push_back(token[i]);

		for(int j = 0;j < mainScheme->tableList.size();j ++)
            //找到了这一列所在的表，将这一列插入这个表的请求序列
            if (mainScheme->tableList[j]->columeType.find(token[i])!= mainScheme->tableList[j]->columeType.end()){
                comList[j].push_back(mainScheme->tableList[j]->columeType[token[i]]->order);
                break;
                //printf("%d\n",mainScheme->tableList[j]->columeType[token[i]]->order);
                //printf("comList =%d \n",j);
                //printf("%s \n",token[i].c_str());
            }
    }
    /*for(int j = 0;j < comList[1].size();j ++)
        printf("com0 %d = %d \n",j,comList[1][j]);
*/
	//获得在结果中的对应位置
	for(int j = 0;j < tmpList.size();j ++)
        ansList[tmpList[j]] = j;
    row.clear();
	row.resize(tmpList.size(), "");


	//获得请求的表
    for (i++; i < token.size(); i++) {
		if (token[i] == "," || token[i] == ";")
			continue;

		if (token[i] == "WHERE")
			break;
		//questTable.push_back(token[i]);
		for(int j = 0;j < mainScheme->tableList.size();j ++)
            if (token[i] == mainScheme->tableList[j]->tableName){
                questTable.push_back(j);
                break;
            }

	}
    //获得条件
	int flag = 0;
	string tarCon;
	Condition tmpCon;
	//printf("before where\n");
	//获得条件
	for (i++; i < token.size();i ++){
        //printf("token is %s \n" ,token[i].c_str());
        if (token[i] == "," || token[i] == "AND")
            continue;
        switch (flag){
            case 0:
                //tmpCon = new Condition;
		//条件的第一部分是列名
		tarCon = token[i];
                break;
            case 1:
		//第二部分是符号
		if (token[i] == "=")
                    tmpCon.kind = 0;
                if (token[i] == "<")
                    tmpCon.kind = 1;
                if (token[i] == ">")
                    tmpCon.kind = 2;
                break;
            case 2:
		//第三部分可以是个常量，也可以是个列名
		tmpCon.strConst = token[i];

                //如果是个列名
                if (token[i][0] != '\'' && (token[i][0] < '0' || token[i][0] > '9')){
                    //printf("token is %s \n" ,token[i].c_str());
                    tmpCon.kind = 3;
                    //新建一个join条件
		    tmpCon.joinCon = new JoinCon;
                    tmpCon.joinCon ->strKey = "";
                    tmpCon.joinCon ->firstCol = "";

                    if (condList.find(token[i]) == condList.end()){
                        vector<Condition> vec1;
                        vec1.clear();
                        condList[token[i]] = vec1;
                    }
                    //加入对应列的条件表
		    condList[token[i]].push_back(tmpCon);
                    for(int j = 0;j < mainScheme->tableList.size();j ++)
                        //加入相应的表的有条件列的表
                        if (mainScheme->tableList[j]->columeType.find(token[i])!= mainScheme->tableList[j]->columeType.end()){
                            condTableList[j].push_back(mainScheme->tableList[j]->columeType[token[i]]->order);
                            //printf("token %d \n" ,j);
                            break;
                        }

                }
		//同样处理等号的右端
                if (condList.find(tarCon) == condList.end()){
                    vector<Condition> vec1;
                    vec1.clear();
                    condList[tarCon] = vec1;
                }
                condList[tarCon].push_back(tmpCon);
                flag = -1;

                for(int j = 0;j < mainScheme->tableList.size();j ++)
                    //be able to find the column
                    if (mainScheme->tableList[j]->columeType.find(tarCon)!= mainScheme->tableList[j]->columeType.end()){
                        condTableList[j].push_back(mainScheme->tableList[j]->columeType[tarCon]->order);
                        //printf("tarcon %d \n" ,j);
                        break;
                    }
                break;
        }
        flag += 1;
	}
    //printf("condTable: %d \n" , condTableList[0][0]);
    //printf("condTable: %d \n" , condTableList[1][0]);
    //printf("%d   %s\n",condList["o_date"][1].kind,condList["o_date"][1].strConst.c_str());
    //erease the repeat
    //printf("after where\n");
    //各个表的有条件列的表的去重
    for(int j = 0;j < mainScheme->tableList.size();j ++)
        if (condTableList[j].size() > 0){
            sort(condTableList[j].begin(), condTableList[j].end() );
            vector<int>::iterator iter_end;
            iter_end = unique(condTableList[j].begin(), condTableList[j].end() );
            condTableList[j].erase(iter_end, condTableList[j].end() );
            //printf("repeat %d  %d\n" ,j,condTableList[j][0]);

        }
    //printf("parser ok\n");




}

//递归执行
void SelectParser::selectExec(int step,vector<string> &result){
    FILE *fin;
	char buf[65536];
	vector<string> column_name, token;
	string str;
	int i;
	//所有表都访问过了
	if (step ==  questTable.size()) {
		str = row[0];
		for (i = 1; i < row.size(); i++)
			str += "," + row[i];
		result.push_back(str);
		return;
	}
	//当前表的编号
	int curTNum = questTable[step];
	//当期表的指针
	Table* curTable = mainScheme->tableList[curTNum];
	//fin = fopen(((string) "data/" + curTable->tableName).c_str(), "r");
    bool flag;
	int curOffset = 0
	int tmpLen;
	char* tarPos = curTable->storeUnit->getData(curOffset , tmpLen);
	while (tarPos != NULL) {
        int len = strlen(buf);
		if (len > 0 && buf[len - 1] == '\n') {
			buf[len - 1] = '\0';
			len--;
		}
		if (len == 0)
			continue;

		split_csv(buf, token);
		flag = true;
		int tmpCol;
		//检查这一列的数据
		for(int i = 0;i < condTableList[curTNum].size();i ++){
            tmpCol = condTableList[curTNum][i];
            if (checkToken(token[tmpCol],condList[curTable->columeName[tmpCol]],curTable->columeName[tmpCol]))
                continue;
            flag = false;
            break;
        }
	//满足条件，将相应列插入的答案中
	if (flag){
            for(int i = 0;i < comList[curTNum].size();i ++){
                tmpCol = comList[curTNum][i];
                row[ansList[curTable->columeName[tmpCol]]] = token[tmpCol];
            }
	    //递归
            selectExec(step + 1,result);
        }
	}
	fclose(fin);

}

//判断
bool SelectParser::checkToken(const string &key,vector<Condition> &rule,const string &colName ){
    vector<int> tmpChange;
    tmpChange.clear();
    int tmp1,tmp2;
    for(int i = 0;i < rule.size();i ++){
        switch (rule[i].kind){
        //equal
            case 0:
                if (! (key == rule[i].strConst))
                    return false;
                break;
            case 1:
                tmp1 = atoi(key.c_str());
                tmp2 = atoi(rule[i].strConst.c_str());
                if (!(tmp1 < tmp2))
                    return false;
                break;
            case 2:
                tmp1 = atoi(key.c_str());
                tmp2 = atoi(rule[i].strConst.c_str());
                if (!(tmp1 > tmp2))
                    return false;
                break;
            //join
            case 3:
                if ( rule[i].joinCon->firstCol != "" && rule[i].joinCon->firstCol != colName && rule[i].joinCon->strKey != key)
                    return false;
                else
                    if (rule[i].joinCon->firstCol == "" || rule[i].joinCon->firstCol == colName)
                        tmpChange.push_back(i);
                break;
        }

    }
//置相应的条件
    for(int i = 0;i < tmpChange.size();i ++){
        rule[tmpChange[i]].joinCon->firstCol = colName;
        rule[tmpChange[i]].joinCon->strKey = key;
    }
    return true;


}




//excute
void SelectParser::execute(vector<string> &result){
   selectExec(0,result);
}
