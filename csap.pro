QT       += core gui sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_NO_DEBUG_OUTPUT
#DEFINES += QT_NO_WARNING_OUTPUT

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dataprocessing.cpp \
    dlgacdmember.cpp \
    dlgaddacd.cpp \
    dlgaddroute.cpp \
    dlgrec.cpp \
    dbaccess.cpp \
    dlgacd.cpp \
    dlgcdr.cpp \
    dlgextensionconf.cpp \
    dlgextnview.cpp \
    dlglogin.cpp \
    dlgrouting.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    dataprocessing.h \
    dbaccess.h \
    dlgacd.h \
    dlgacdmember.h \
    dlgaddacd.h \
    dlgaddroute.h \
    dlgcdr.h \
    dlgextensionconf.h \
    dlgextnview.h \
    dlglogin.h \
    dlgrec.h \
    dlgrouting.h \
    mainwindow.h

FORMS += \
    dlgacd.ui \
    dlgacdmember.ui \
    dlgaddacd.ui \
    dlgaddroute.ui \
    dlgcdr.ui \
    dlgextensionconf.ui \
    dlgextnview.ui \
    dlglogin.ui \
    dlgrec.ui \
    dlgrouting.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
