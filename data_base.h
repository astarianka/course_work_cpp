#ifndef DATA_BASE_H
#define DATA_BASE_H
#define blockSize 100
#define recordAmount 10000
#define blockAmount 100
#define recordRange 150
#include <string>
using namespace std;
class DataBase {
    struct KeyNum {
        int key;
        int recNum;
        bool busy = false;
    };
    struct KeyData {
        int key;
        string data;
        bool busy = false;
    };
    int maxNumRec;
    struct KeyNum** indexScope;
    struct KeyData* mainScope;
    int* rangeArray;

private:
    int checkUnic(int index, int key);
    int findInsertPosition(int index, int key, int last);
    int findCurRec();
    int findRecIndex(int block, int left, int right, int key);
    void deleteIndexRec(int block, int recIndex);
    void insertIntoBlock(int block, int curRec, int key);
    int findLastIndex(int block);
    int findRecNum(int key);
    int findBlock(int left, int right, int key);

public:
    DataBase();
    DataBase(string indexFile, string mainFile);
    string searchRecInfo(int key);
    string searchRecData(int key);
    void insertRandomValues(int amount);
    void insert(int key, string data);
    int findFreeKey();
    void update(int key, string data);
    void deleteRec(int key);
    void showMainTable();
    void showIndexTable();
    void save(string indexFile, string mainFile);
};

#endif //DATA_BASE_H

