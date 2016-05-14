TEMPLATE = app

QT += qml quick widgets network

SOURCES += main.cpp \
    udpservice.cpp \
    tcpservice.cpp \
    clipboardservice.cpp \
    fileservice.cpp \
    sessionmanager.cpp \
    udppackage.cpp \
    roomservice.cpp \
    encryptionservice.cpp \
    tcpthreadedsender.cpp \
    GUI/loginpassdialog.cpp \
    GUI/roomchoose.cpp \
    GUI/addroom.cpp

RESOURCES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

HEADERS += \
    udpservice.h \
    tcpservice.h \
    tcppackages.h \
    shared_clipboard_exception.h \
    clipboardservice.h \
    fileservice.h \
    common.h \
    sessionmanager.h \
    udppackage.h \
    roomservice.h \
    encryptionservice.h \
    tcpthreadedsender.h \
    GUI/loginpassdialog.h \
    GUI/roomchoose.h \
    GUI/addroom.h

CONFIG += c++14



DISTFILES +=

FORMS += \
    GUI/loginpassdialog.ui \
    GUI/roomchoose.ui \
    GUI/addroom.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/cryptopp/release/ -lcryptopp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/cryptopp/debug/ -lcryptopp
else:unix: LIBS += -L$$PWD/cryptopp/ -lcryptopp

INCLUDEPATH += $$PWD/cryptopp
DEPENDPATH += $$PWD/cryptopp

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/cryptopp/release/libcryptopp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/cryptopp/debug/libcryptopp.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/cryptopp/release/cryptopp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/cryptopp/debug/cryptopp.lib
else:unix: PRE_TARGETDEPS += $$PWD/cryptopp/libcryptopp.a
