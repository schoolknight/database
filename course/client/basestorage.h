#ifndef BASESTORAGE_H_INCLUDED
#define BASESTORAGE_H_INCLUDED
#include<string>
#include<vector>
#include<cstring>
#include <stdio.h> 
#include <stdlib.h>


using namespace std;


class BaseStorage{
    public:
        BaseStorage(){};
        //get data
        virtual int getData(int offSet,char* tarPos){};
        //write data
        virtual int writeData(const string &key,const vector<int> &type){};
        ~BaseStorage(){};
};



#endif // BASESTORAGE_H_INCLUDED
