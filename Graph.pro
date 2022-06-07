#QT -= gui
QT += network
QT+=core
CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        barrier.cpp \
        car.cpp \
        gamemap.cpp \
        locationmap.cpp \
        main.cpp \
        myexception.cpp \
        mygraph.cpp \
        mylog.cpp \
        mytcpsocket.cpp \
        point.cpp \
        settings.cpp

INCLUDEPATH += /usr/include/boost

INCLUDEPATH += /home/NAMI/ila.solomatin/Downloads/ogre-13.3.1/OgreMain/include/

unix:!macx: LIBS += -L$$PWD/../Downloads/ogre-13.3.1/build/lib/ -lOgreMain

INCLUDEPATH += $$PWD/../Downloads/ogre-13.3.1/build/include
DEPENDPATH += $$PWD/../Downloads/ogre-13.3.1/build/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    barrier.h \
    car.h \
    gamemap.h \
    locationmap.h \
    myexception.h \
    mygraph.h \
    mylog.h \
    mytcpsocket.h \
    point.h \
    settings.h

unix:!macx: LIBS += -L$$PWD/../../Downloads/ogre-13.3.1/build/lib/ -lOgreBites

INCLUDEPATH += $$PWD/../../Downloads/ogre-13.3.1/build/include
DEPENDPATH += $$PWD/../../Downloads/ogre-13.3.1/build/include

DISTFILES +=
