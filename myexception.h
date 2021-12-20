#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H
#include <iostream>

#define PathNotFound 0
#define ConfigNotFound -1
//#define ConfigNotFound -1
#define DataPackageError -3
//#define ConfigNotFound -1


class MyException : public std::exception
{
private:
    int ErrorCode;
    char* msg;
public:
    MyException(const char *msg);
    MyException(int ErrorCode);
    MyException(const char *msg,int ErrorCode);
    ~MyException();
    int GetErrorCode();
    void SetErrorCode(int ErrorCode);
    const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
};

#endif // MYEXCEPTION_H
