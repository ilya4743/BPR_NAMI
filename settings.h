#ifndef SETTINGS_H
#define SETTINGS_H

#include <QVariant>
#include <QSettings>
#include <QMetaEnum>

class Settings{
    Q_GADGET
    Q_ENUMS(Section)
    Q_ENUMS(Key)
public:
    enum Section{
        General,
        Network,
        Debug
    };

    enum Key{
        IP_SERVER,
        PORT,
        DEBUG_OUTPUT
    };

    class ValueRef{
    public:
        ValueRef(Settings &st, const QString &kp) :
            parent(st), keyPath(kp){}
        ValueRef & operator = (const QVariant &d);
    private:
        Settings &parent;
        const QString keyPath;
    };

    static void setDefaults(const QString &str);
    static QVariant get(Key, Section /*s*/ = General);
    static ValueRef set(Key, Section /*s*/ = General);

private:
    QString keyPath(Section, Key);

    static Settings & instance();
    QMetaEnum keys;
    QMetaEnum sections;
    QMap<QString, QVariant> defaults;
    QSettings conf;

    Settings();
    Settings(const Settings &);
    Settings & operator = (const Settings &);
};

#endif // SETTINGS_H
