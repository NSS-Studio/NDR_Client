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
<<<<<<< HEAD:client/bigdata/bigdata.pri

win32:LIBS += -l Iphlpapi
=======
win32:LIBS += -lIphlpapi -lWs2_32
>>>>>>> 86a50c76f18229dfe21e583640e936e57449e499:client/information/information.pri
