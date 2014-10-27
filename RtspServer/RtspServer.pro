TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Rtspserver/RTSPCommon.cpp \
    Rtspserver/RTSPServer.cpp \
    NetServer/TCPServer.cpp \
    include/Locale.cpp \
    include/strDup.cpp \
    ServerMediaSession/ServerMediaSession.cpp \
    include/inet.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Rtspserver/RTSPCommon.hh \
    Rtspserver/RTSPServer.h \
    NetServer/TCPServer.h \
    include/Boolean.hh \
    include/GroupsockHelper.hh \
    include/HashTable.hh \
    include/Locale.hh \
    include/NetAddress.hh \
    include/NetCommon.h \
    include/strDup.hh \
    include/UsageEnvironment.hh \
    include/UsageEnvironment_version.hh \
    libuv/include/android-ifaddrs.h \
    libuv/include/pthread-fixes.h \
    libuv/include/stdint-msvc2008.h \
    libuv/include/tree.h \
    libuv/include/uv-aix.h \
    libuv/include/uv-bsd.h \
    libuv/include/uv-darwin.h \
    libuv/include/uv-errno.h \
    libuv/include/uv-linux.h \
    libuv/include/uv-sunos.h \
    libuv/include/uv-threadpool.h \
    libuv/include/uv-unix.h \
    libuv/include/uv-version.h \
    libuv/include/uv-win.h \
    libuv/include/uv.h \
    ServerMediaSession/ServerMediaSession.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/release/ -luv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/debug/ -luv
else:unix: LIBS += -L$$PWD/../../../../usr/lib64/ -luv

INCLUDEPATH += $$PWD/../../../../usr/lib64
DEPENDPATH += $$PWD/../../../../usr/lib64
