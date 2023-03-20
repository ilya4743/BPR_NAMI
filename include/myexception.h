#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H
#include <string>

/// Путь не найден исключений
#define PathNotFound 0

/// Конфигурационный файл не найден
#define ConfigNotFound -1

/// Неправильный формат пакета с данными
#define DataPackageError -3

/// Класс исключений
class MyException : public std::exception {
   private:
    /// Код ошибки
    int ErrorCode;

    /// Сообщение об ошибке
    std::string msg;

   public:
    MyException() : std::exception() {}
    MyException(std::string& msg, int code) : std::exception(), msg(msg), ErrorCode(code) {}
    MyException(const char* cmsg, int code) : std::exception(), msg(cmsg), ErrorCode(code) {}
    int GetErrorCode() { return ErrorCode; }
    const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override { return this->msg.c_str(); }
};

#endif  // MYEXCEPTION_H
