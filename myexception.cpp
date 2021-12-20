#include "myexception.h"

MyException::MyException(const char *msg):exception()
{
    std::ostream os(NULL);
    os<<msg;
}

MyException::MyException(int ErrorCode):exception()
{
    this->ErrorCode=ErrorCode;
}
MyException::MyException(const char *msg,int ErrorCode):exception()
{
    std::ostream os(NULL);
    os<<msg;
    this->ErrorCode=ErrorCode;
}
MyException::~MyException()
{

}
int MyException::GetErrorCode()
{
    return ErrorCode;
}
void MyException::SetErrorCode(int ErrorCode)
{
    this->ErrorCode=ErrorCode;
}

const char* MyException::what()const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
{
    return msg;
}
