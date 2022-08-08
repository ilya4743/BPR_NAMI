#include <QCoreApplication>
#include <QFile>
#include "mytcpsocket.h"
#include "settings.h"
#include "myexception.h"
#include "logger.h"

#include <QDir>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>

// Умный указатель на файл логирования
QScopedPointer<QFile>   m_logFile;

// Объявляение обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

// Реализация обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile.data());
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }
    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": "
        << msg<<'\n';
    out.flush();    // Очищаем буферизированные данные
}


/**
@mainpage Блок принятия решений Документация
@details Прив <b> рврвар </b>
*/

int main(int argc, char *argv[])
{
    try
    {
        QCoreApplication a(argc, argv);
        QFile f("config.cfg");
        if(!f.open(QIODevice::ReadOnly))
            throw MyException("config.cfg not found", ConfigNotFound);
        Settings::setDefaults(f.readAll());
        // Пишем ли логи
        if(Settings::get(Settings::PRINT_LOG).toBool())
        {
            m_logFile.reset(new QFile("log.txt"));
            // Открываем файл логирования
            m_logFile.data()->open(QFile::Append | QFile::Text);
            // Устанавливаем обработчик
            qInstallMessageHandler(messageHandler);
        }
        else
            qInstallMessageHandler(+[](QtMsgType, const QMessageLogContext &, const QString &){});
        int DEBUG_OUTPUT=Settings::get(Settings::DEBUG_OUTPUT).toInt();
        f.close();
        MyTcpSocket s;
        s.doConnect(Settings::get(Settings::IP_SERVER).toString(),Settings::get(Settings::PORT).toInt(),
        DEBUG_OUTPUT, Settings::get(Settings::PRINT_LOG).toBool(), Settings::get(Settings::RECONNECT_TIME).toInt());

        return a.exec();
    }  catch (MyException& Ex)
    {
        qDebug()<<Ex.what();
        qDebug()<<"Error "<<Ex.GetErrorCode();
    }
}
