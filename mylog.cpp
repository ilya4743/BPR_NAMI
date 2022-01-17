#include "mylog.h"

void MyLog::setLog(bool b)
{
    flag_log  = b;
}

void MyLog::setFilename(char* filename)
{
    this->filename=filename;
}

void MyLog::printLog(char* str)
{
    if(flag_log)
        *os << str;
}

template<>
void MyLog::printLogToFile( const char* o)
{
    if(flag_log)
        *ofs<<o<<endl;
}

template<class T>
void MyLog::printLogToFile( T* o)
{
    if(flag_log)
        *ofs<<*o<<endl;
}

template<class T>
ofstream& operator<<(ofstream &out, const T &o)
{
    out<<o;
    return o;
}

MyLog::MyLog(bool flag,char*filename)
{
    os=new iostream(NULL);
    flag_log = flag;
    this->filename=filename;
    ofs=new ofstream(this->filename, std::ofstream::out);
}

MyLog::~MyLog()
{
    delete os;
    delete ofs;
}
