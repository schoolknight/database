#include"hybridstorage.h"


HybridStorage::HybridStorage(const string& keyName){
    //printf("0\n");
    fileName = keyName;
    //buffer = new char[bufferLen];
    buffer = NULL;
    memUsed = 0;
    memLimit = bufferLen;
    curOffset = 0;
    lenFile = 0;
    //printf("%d\n");
    fout = fopen(fileName.c_str(), "w");
    fclose(fout);
    //printf("0\n");
}

void HybridStorage::writeStart(){
    fout = fopen(fileName.c_str(), "a+");
}

void HybridStorage::writeFinish(){
    fclose(fout);
}

int HybridStorage::writeData(vector<string> &key,const vector<int> &type){
    int ans = curOffset;
    char buff[1000];
    int tmplen = 0;
    int tmpInt;
    for(int i = 0;i < key.size();i ++){
        //INTEGER
        if (type[i] == 0){
            tmpInt = atoi(key[i].c_str());
            memcpy(buff + curOffset,&tmpInt,4);
            curOffset += 4;
        }
        //varchar
        else{
            tmpInt = key[i].length();
            buff[curOffset] = (uchar)tmpInt;
            curOffset ++;
            memcpy(buff + curOffset,key[i].c_str(),tmpInt);
            curOffset += tmpInt;
        }
    }
    fwrite(buff,curOffset - ans,1,fout);
    return ans;
}
int HybridStorage::moveToMem(char* tarPos,int totMem){
    //内存足够大
    buffer = tarPos;
    if (curOffset < totMem){
        if (curOffset + STD_BUFF < totMem)
            memLimit = curOffset + STD_BUFF;
        else
            memLimit = totMem;

        fout = fopen(fileName.c_str(), "rb+");
        fread(buffer,1,curOffset,fout);
        fclose(fout);
        return totMem - memLimit;
    }
    //内存不够了
    else{
        //buffer = new char[totMem];
        fout = fopen(fileName.c_str(), "rb+");
        fread(buffer,1,totMem,fout);
        fclose(fout);
        memLimit = totMem;
        return 0;
    }

}
