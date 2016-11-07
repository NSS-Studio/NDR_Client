INCLUDEPATH +=  $$PWD
DEPENDPATH +=   $$PWD

HEADERS += \
    $$PWD/interfaceinfo.h \
    $$PWD/userinfo.h \
    $$PWD/osinfo.h

SOURCES += \
    $$PWD/interfaceinfo.cpp \
    $$PWD/userinfo.cpp \
    $$PWD/osinfo.cpp
win32:LIBS += -lIphlpapi -lWs2_32
