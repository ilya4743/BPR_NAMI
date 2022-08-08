#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H
#include <iostream>

/// Путь не найден исключений
#define PathNotFound 0

/// Конфигурационный файл не найден
#define ConfigNotFound -1

/// Неправильный формат пакета с данными
#define DataPackageError -3

/// Класс исключений
class MyException : public std::exception
{
private:
    /// Код ошибки
    int ErrorCode;

    /// Сообщение об ошибке
    const char* msg;
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
