#ifndef MYLOG_H
#define MYLOG_H
#include <iostream>
#include<fstream>
#include<QString>
#include<QTime>
#include"gamemap.h"
using namespace std;
class GameMap;
class MyLog
{
private:
    /* data */
    iostream *os;
    //ofstream *ofs;
    bool flag_log;
    char* filename;

public:

    void setLog(bool b);
    void setFilename(char* filename);
    ofstream *ofs;

    MyLog(bool flag,char*filename);
    ~MyLog();
    void printLog(char* str);
    template<class T>
    void printLogToFile( T *o);
    //template<>
    void printLogToFile( const char* o);
    //template<>
    void printLogToFile(const GameMap& o);

    //template<class T>
    //friend ofstream& operator<<(ofstream &out, const T &o);
};

#endif // MYLOG_H
