INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS +=  $$PWD/basedsl.h \
            $$PWD/basedsl.hpp \
            $$PWD/pppoe.hpp \
            $$PWD/basedslfactory.hpp

SOURCES +=  $$PWD/pppoe.cpp \
            $$PWD/basedsl.cpp \
            $$PWD/basedslfactory.cpp

win32 {
    SOURCES += $$PWD/basedsl_win.cpp
    LIBS    +=  -luser32 \
                -lrasapi32
}

unix:!macx {
    HEADERS += $$PWD/linuxbasedsl.hpp
    SOURCES += $$PWD/linuxbasedsl.cpp
    QT      += dbus
    INCLUDEPATH += /usr/include/NetworkManager
}

macx {
    HEADERS += $$PWD/macosbasedsl.hpp
    SOURCES += $$PWD/macosbasedsl.cpp
    LIBS += -framework SystemConfiguration \
            -framework CoreFoundation \
            -framework Security \
            -framework Carbon
}
