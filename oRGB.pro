#-------------------------------------------------
#
# Project created by QtCreator 2018-08-11T02:23:24
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = oRGB
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    converter.cpp

HEADERS += \
        mainwindow.h \
    converter.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Downloads/opencv/build/x64/vc15/lib/ -lopencv_world342
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Downloads/opencv/build/x64/vc15/lib/ -lopencv_world342d

INCLUDEPATH += $$PWD/../../Downloads/opencv/build/include
DEPENDPATH += $$PWD/../../Downloads/opencv/build/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Downloads/opencv/build/x64/vc15/lib/ -lopencv_world342
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Downloads/opencv/build/x64/vc15/lib/ -lopencv_world342d

INCLUDEPATH += $$PWD/../../Downloads/opencv/build/include
DEPENDPATH += $$PWD/../../Downloads/opencv/build/include
