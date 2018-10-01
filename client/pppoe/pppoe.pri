INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS +=  $$PWD/basedsl.hpp \
            $$PWD/pppoe.hpp \
            $$PWD/basedslfactory.hpp

SOURCES +=  $$PWD/pppoe.cpp \
            $$PWD/basedsl.cpp \
            $$PWD/basedslfactory.cpp

win32 {
    HEADERS += $$PWD/windowsbasedsl.hpp
    SOURCES += $$PWD/windowsbasedsl.cpp
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
