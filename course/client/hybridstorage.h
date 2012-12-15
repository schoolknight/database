#ifndef HYBRIDSTORAGE_H_INCLUDED
#define HYBRIDSTORAGE_H_INCLUDED

#include"basestorage.h"
#include<string>
#include <cstdio>
#include <cstdlib>
#include"common.h"


class HybridStorage: public BaseStorage{
    private:
        string fileName;
        char* buffer;
	char blockBuffer[BLOCK_LEN];
	int blockStart;        
	int memLimit;
        //int memUsed;
        //int lenFile;
        int curOffset;
        FILE* fout;
    public:
        HybridStorage(const string& keyName);
        char* getData(int offSet,int& len);
        //write data
        void writeStart();
        int writeData(vector<string> &key,const vector<int> &type);
        void writeFinish();
        int moveToMem(char* tarPos,int totMem);
};

#endif // HYBRIDSTORAGE_H_INCLUDED
