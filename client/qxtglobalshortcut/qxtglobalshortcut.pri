INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/qxtglobal.h \
           $$PWD/qxtglobalshortcut.h \
           $$PWD/qxtglobalshortcut_p.h
SOURCES += $$PWD/qxtglobalshortcut.cpp
win32{
    SOURCES += $$PWD/qxtglobalshortcut_win.cpp
    LIBS += -luser32
}
unix:!macx: {
    SOURCES += $$PWD/qxtglobalshortcut_x11.cpp
    LIBS    += -lX11
}
macx: SOURCES += $$PWD/qxtglobalshortcut_mac.cpp
