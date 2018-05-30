#include "data_base.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <climits>
#define blockSize 100
#define expectedRecAmount 10000
#define blockAmount 100
#define recordRange 150

int main()
{

    DataBase* base = NULL;

    cout << " ------choose an action------ " << endl;
    cout << " 1 -----open base from file-----" << endl;
    cout << " 2 -----create new base-----" << endl;
    int number = 10;
    int num = 10;
    string indexFile = "";
    string mainFile = "";
    cin >> number;
    if (number == 1) {

        cout << "enter the name of INDEX scope file " << endl;
        cin >> indexFile;
        cout << "enter the name of MAIN scope file " << endl;
        cin >> mainFile;
        base = new DataBase(indexFile, mainFile);
    }
    else if (number == 2) {
        base = new DataBase;
    }
    else {
        cout << "choose an action from the list!" << endl;
    }

    while (num != 0) {
        cout << endl;
        cout << " ------choose an action------ " << endl;
        cout << " ----1---- insert random values " << endl;
        cout << " ----2---- insert user's data " << endl;
        cout << " ----3---- find free record key " << endl;
        cout << " ----4---- update record " << endl;
        cout << " ----5---- delete record " << endl;
        cout << " ----6---- search record data " << endl;
        cout << " ----7---- search full record info " << endl;
        cout << " ----8---- show main scope " << endl;
        cout << " ----9---- show index scope " << endl;
        cout << " ----10---- save changes " << endl;
        cout << " ----11---- save changes to selected files " << endl;
        cout << " ----0---- close base " << endl;
        cin >> num;
        switch (num) {
        case 1: {
            cout << " *INSERTION* " << endl;
            cout << " How many records you want to insert?" << endl;
            int amount;
            cin >> amount;
            base->insertRandomValues(amount);
            break;
        }

        case 2: {
            cout << " *INSERTION* " << endl;
            string answer = "y";
            while (answer == "y") {
                cout << " Insert key" << endl;
                int key;
                cin >> key;
                cout << " Insert data " << endl;
                string data;
                cin >> data;
                base->insert(key, data);
                cout << " Do you want to insert more? (y/n) ";
                cin >> answer;
            }

            break;
        }
        case 3: {
            cout << " *FREE KEY* " << endl;
            cout << base->findFreeKey() << endl;
            break;
        }
        case 4: {
            int key;
            string data;
            cout << " *UPDATING* " << endl;
            cout << " Enter the key of record" << endl;
            cin >> key;
            cout << " Enter new data for record" << endl;
            cin >> data;
            base->update(key, data);
            break;
        }
        case 5: {
            int key;
            cout << " *DELETION* " << endl;
            cout << " Enter the key of record" << endl;
            cin >> key;
            base->deleteRec(key);
            break;
        }
        case 6: {
            int key;
            cout << " *DATA SEARCH* " << endl;
            cout << " Enter the key of record" << endl;
            cin >> key;
            cout << base->searchRecData(key) << endl;
            break;
        }
        case 7: {
            int key;
            cout << " *INFO SEARCH* " << endl;
            cout << " Enter the key of record" << endl;
            cin >> key;
            cout << base->searchRecInfo(key) << endl;
            break;
        }
        case 8: {
            cout << " *MAIN TABLE* " << endl;
            base->showMainTable();
            break;
        }
        case 9: {
            cout << " *INDEX TABLE* " << endl;
            base->showIndexTable();
            break;
        }
        case 10: {
            cout << " *SAVING* " << endl;
            if (number == 1) {
                base->save(indexFile, mainFile);
            }
            else {
                cout << "ERROR: you have to choose files before (action № 11)" << endl;
            }

            break;
        }
        case 11: {
            cout << " *SAVING* " << endl;
            cout << " Enter index scope file name" << endl;
            cin >> indexFile;
            cout << " Enter main scope file name" << endl;
            cin >> mainFile;
            base->save(indexFile, mainFile);
            break;
        }
        case 0: {
            break;
        }
        default: {
            cout << "ERROR: choose action from the list!" << endl;
        }
        }
    }
}

DataBase::DataBase()
{
    maxNumRec = 0;
    mainScope = new KeyData[expectedRecAmount];
    indexScope = new KeyNum*[blockAmount + 1];
    rangeArray = new int[blockAmount];
    int maxValue = recordRange;
    for (int j = 0; j < blockAmount; j++) {
        rangeArray[j] = maxValue;
        maxValue += recordRange;
    }
    for (int i = 0; i < blockAmount; ++i)
        indexScope[i] = new KeyNum[blockSize + 1];

    indexScope[blockAmount] = new KeyNum[expectedRecAmount / 2];
}
DataBase::DataBase(string indexFile, string mainFile)
{

    mainScope = new KeyData[expectedRecAmount];
    indexScope = new KeyNum*[blockAmount + 1];
    for (int i = 0; i < blockAmount; ++i)
        indexScope[i] = new KeyNum[blockSize];

    rangeArray = new int[blockAmount];
    int maxValue = recordRange;
    for (int j = 0; j < blockAmount; j++) {
        rangeArray[j] = maxValue;
        maxValue += recordRange;
    }
    indexScope[blockAmount] = new KeyNum[expectedRecAmount / 2];
    int i = 0;
    string line;
    string key;
    string data;
    ifstream mFile(mainFile);
    if (mFile.is_open()) {
        while (!mFile.eof()) {
            getline(mFile, line);
            istringstream iss(line);
            iss >> key;
            mainScope[i].key = atoi(key.c_str());
            if (getline(mFile, data)) {
                mainScope[i].data = data;
            }
            mainScope[i].busy = true;
            i++;
        }
        mFile.close();
    }
    else
        cout << "Unable to open file";

    maxNumRec = i;

    string recNum;
    int amount;
    int num;
    string info;
    ifstream iFile(indexFile);
    if (iFile.is_open()) {
        while (!iFile.eof()) {
            getline(iFile, line);
            istringstream iss(line);
            iss >> info;
            if (info == "block") {
                iss >> info;
                num = atoi(info.c_str());
                iss >> info;
                amount = atoi(info.c_str());
            }
            for (int k = 0; k < amount; k++) {
                getline(iFile, line);
                istringstream iss(line);
                iss >> key;
                indexScope[num][k].key = atoi(key.c_str());
                iss >> recNum;
                indexScope[num][k].recNum = atoi(recNum.c_str());
                indexScope[num][k].busy = true;
            }
        }

        iFile.close();
    }
    else
        cout << "Unable to open file";
}

void DataBase::save(string indexFile, string mainFile)
{
    ofstream mFile;
    mFile.open(mainFile, ios::out);
    int i = 0;
    int num = maxNumRec;
    for (int i = 0; i < expectedRecAmount; i++) {

        if (mainScope[i].busy == true && (num > 0)) {
            mFile << to_string(mainScope[i].key) << endl;
            mFile << mainScope[i].data << endl;
            num--;
        }
        if (num == 1) {
            mFile << to_string(mainScope[i + 1].key) << endl;
            mFile << mainScope[i + 1].data;
        }
        if (num - 1 == 0) {
            mFile.close();
        }
    }
    if (maxNumRec - expectedRecAmount > 0) {
        for (int i = expectedRecAmount; i < maxNumRec; i++) {

            if (mainScope[i].busy == true && (num > 0)) {
                mFile << to_string(mainScope[i].key) << endl;
                mFile << mainScope[i].data << endl;
                num--;
            }
            if (num == 1) {
                mFile << to_string(mainScope[i + 1].key) << endl;
                mFile << mainScope[i + 1].data;
            }
            if (num - 1 == 0) {
                mFile.close();
            }
        }
    }
    ofstream iFile;
    iFile.open(indexFile, ios::out);
    int amount = 0;
    for (int i = 0; i < blockAmount; i++) {
        if (indexScope[i][0].busy == true) {
            iFile << "block " << to_string(i) << " ";
        }
        for (int k = 0; k < blockSize; k++) {
            if (indexScope[i][k].busy == true) {
                amount += 1;
            }
        }
        if (indexScope[i][0].busy == true) {
            iFile << amount << endl;
        }
        amount = 0;
        for (int j = 0; j < blockSize; j++) {
            if (indexScope[i][j].busy == true) {

                iFile << indexScope[i][j].key << "  " << indexScope[i][j].recNum << endl;
            }
        }
    }
    amount = 0;
    if (indexScope[blockAmount][0].busy == true) {
        while (indexScope[blockAmount][amount].busy == true) {
            amount += 1;
        }
        iFile << "block " << to_string(blockAmount) << " " << amount << endl;
        for (int k = 0; k < amount - 1; k++) {
            iFile << indexScope[blockAmount][k].key << "  " << indexScope[blockAmount][k].recNum << endl;
        }
        iFile << indexScope[blockAmount][amount - 1].key << "  " << indexScope[blockAmount][amount - 1].recNum;
    }

    iFile.close();
    return;
}
void DataBase::insert(int key, string data)
{
    int block = findBlock(0, blockAmount - 1, key);
    int last = findLastIndex(block);
    if (last == blockSize) {
        if (checkUnic(block, key) == 0) {
            block = blockAmount;
        }
    }
    if (checkUnic(block, key) == 0) {

        int curRec = findCurRec();
        mainScope[curRec].key = key;
        mainScope[curRec].data = data;
        mainScope[curRec].busy = true;
        insertIntoBlock(block, curRec, key);

        maxNumRec++;
        if (maxNumRec > expectedRecAmount) {
            cout << "Attention! The base is full. Its work may be slower. It is better to create new base." << endl;
        }
        return;
    }
    cout << " key is not unic " << endl;
}
int DataBase::findCurRec()
{

    for (int i = 0; i < expectedRecAmount; i++) {
        if (mainScope[i].busy == false) {
            return i;
        }
    }
    for (int i = expectedRecAmount;; i++) {
        if (mainScope[i].busy == false) {
            return i;
        }
    }
}

int DataBase::findBlock(int left, int right, int key)
{

    if (right >= left) {
        int mid = left + (right - left) / 2;

        if (rangeArray[mid] >= key && rangeArray[mid] - recordRange <= key) {

            return mid;
        }

        if (rangeArray[mid] > key)
            return findBlock(left, mid - 1, key);

        return findBlock(mid + 1, right, key);
    }
    return blockAmount;
}
int DataBase::findLastIndex(int block)
{
    int j = 0;
    while (indexScope[block][j].busy == true) {
        j++;
    }
    return j;
}
int DataBase::findInsertPosition(int block, int key, int last)
{
    if (key < indexScope[block][0].key) {
        return 0;
    }
    for (int i = 0; i < last - 1; i++) {
        if ((indexScope[block][i].key < key) && (indexScope[block][i + 1].key > key)) {
            return i + 1;
        }
    }
    return last;
}
void DataBase::insertIntoBlock(int block, int curRec, int key)
{
    int last = findLastIndex(block);
    if (last == blockSize) {
        block = blockAmount;
        last = findLastIndex(block);
    }
    int pos = findInsertPosition(block, key, last);
    for (int i = last; i > pos; i--) {
        indexScope[block][i].key = indexScope[block][i - 1].key;
        indexScope[block][i].recNum = indexScope[block][i - 1].recNum;
        indexScope[block][i].busy = true;
    }
    indexScope[block][pos].key = key;
    indexScope[block][pos].recNum = curRec;
    indexScope[block][pos].busy = true;
    indexScope[block][last + 1].busy = false;
}
void DataBase::insertRandomValues(int amount)
{

    srand(time(0));
    if (amount > expectedRecAmount) {
        cout << "Attention! The amount is too big. Base work may be slower. It is better to create new base or split records into two bases." << endl;
        return;
    }
    string valueArray[10] = { "apple", "banana", "orange", "apricot", "cherry", "kiwifruit", "lemon", "lime", "mango", "pear" };
    string recData = "";
    int key;
    int randIndex;
    int flag = 1;
    int last;
    for (int j = 0; j < amount; j++) {

        while (flag > 0) {
            key = 1 + rand() % (recordRange * blockAmount);
            int block = findBlock(0, blockAmount - 1, key);
            last = findLastIndex(block);
            if (last == blockSize) {
                if (checkUnic(block, key) == 0) {
                    block = blockAmount;
                }
            }
            if (checkUnic(block, key) == 0) {
                for (int i = 0; i < 3; i++) {
                    randIndex = rand() % 10;
                    recData.append(valueArray[randIndex]);
                    recData.append(" ");
                }
                insert(key, recData);
                flag--;
                recData = "";
            }
        }
        flag++;
    }
}

string DataBase::searchRecData(int key)
{
    int block = findBlock(0, blockAmount - 1, key);
    if (checkUnic(block, key) == -1 || checkUnic(blockAmount, key) == -1) {
        int recNum = findRecNum(key);
        return mainScope[recNum].data;
    }
    return "key doesn't exist";
}
string DataBase::searchRecInfo(int key)
{
    int block = findBlock(0, blockAmount - 1, key);
    if (checkUnic(block, key) == -1 || checkUnic(blockAmount, key) == -1) {
        string info = "";
        int recNum = findRecNum(key);
        info.append("Record №: ");
        info.append(to_string(recNum));
        info.append("  ");
        int block = findBlock(0, blockAmount - 1, key);
        if (checkUnic(block, key) == 0) {
            block = blockAmount;
        }
        info.append("Index Block: ");
        info.append(to_string(block));
        info.append("  ");
        info.append("Key: ");
        info.append(to_string(key));
        info.append("  ");
        info.append("Data: ");
        info.append(mainScope[recNum].data);
        return info;
    }
    return "key doesn't exist";
}
int DataBase::findRecNum(int key)
{
    int block = findBlock(0, blockAmount - 1, key);
    if (checkUnic(block, key) == 0) {
        block = blockAmount;
    }
    int lastIndex = findLastIndex(block);
    int recIndex = findRecIndex(block, 0, lastIndex, key);
    return indexScope[block][recIndex].recNum;
}
void DataBase::update(int key, string data)
{
    int block = findBlock(0, blockAmount - 1, key);
    if (checkUnic(block, key) == 0 && checkUnic(blockAmount, key) == 0) {
        cout << "key doesn't exist" << endl;
        return;
    }
    int rec = findRecNum(key);
    mainScope[rec].data = data;
    return;
}
void DataBase::deleteRec(int key)
{
    int block = findBlock(0, blockAmount - 1, key);
    if (checkUnic(block, key) == 0) {
        if (checkUnic(blockAmount, key) == 0) {
            cout << "key doesn't exist" << endl;
            return;
        }
        block = blockAmount;
    }
    int lastIndex = findLastIndex(block);
    int recIndex = findRecIndex(block, 0, lastIndex, key);
    int recNum = indexScope[block][recIndex].recNum;
    deleteIndexRec(block, recIndex);
    mainScope[recNum].busy = false;
    maxNumRec -= 1;
}
void DataBase::deleteIndexRec(int block, int recIndex)
{
    int lastIndex = findLastIndex(block);
    for (int i = recIndex; i < lastIndex - 1; i++) {
        indexScope[block][i].key = indexScope[block][i + 1].key;
        indexScope[block][i].recNum = indexScope[block][i + 1].recNum;
    }
    indexScope[block][lastIndex - 1].busy = false;
}

int DataBase::findRecIndex(int block, int left, int right, int key)
{

    if (right >= left) {
        int mid = left + (right - left) / 2;

        if (indexScope[block][mid].key == key)
            return mid;

        if (indexScope[block][mid].key > key)
            return findRecIndex(block, left, mid - 1, key);

        return findRecIndex(block, mid + 1, right, key);
    }
    return -1;
}
int DataBase::findFreeKey()
{
    if (maxNumRec < expectedRecAmount) {
        while (true) {
            srand(time(0));
            int key;
            int block;
            key = 1 + rand() % (recordRange * blockAmount);
            block = findBlock(0, blockAmount - 1, key);
            if (checkUnic(block, key) == 0 && checkUnic(blockAmount, key) == 0) {
                return key;
            }
        }
    }
    cout << "Attention! The base is full. Its work may be slower. It is better to create new base." << endl;
    return -1;
}

void DataBase::showIndexTable()
{
    for (int i = 0; i < blockAmount; i++) {
        if (indexScope[i][0].busy == true) {
            cout << " block " << i << endl;
        }
        for (int j = 0; j < blockSize; j++) {
            if (indexScope[i][j].busy == true) {
                cout << "  j: " << j << " key: " << indexScope[i][j].key << " recNum:  " << indexScope[i][j].recNum << endl;
            }
        }
    }

    if (indexScope[blockAmount][0].busy == true) {
        cout << " block " << blockAmount << endl;
        int m = 0;
        while (indexScope[blockAmount][m].busy == true) {
            cout << "  j: " << m << " key: " << indexScope[blockAmount][m].key << " recNum:  " << indexScope[blockAmount][m].recNum << endl;
            m++;
        }
    }
}
void DataBase::showMainTable()
{
    int i = 0;
    int num = maxNumRec;
    for (int i = 0; i < maxNumRec; i++) {
        if (mainScope[i].busy == true) {
            cout << mainScope[i].key << " " << mainScope[i].data << endl;
            num--;
        }
        if (num = 0) {
            return;
        }
    }
}
int DataBase::checkUnic(int block, int key)
{
    int j = 0;
    while (indexScope[block][j].busy == true) {
        if (indexScope[block][j].key == key) {
            return -1;
        }
        j++;
    }
    return 0;
}

