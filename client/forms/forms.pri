QT += widgets gui xml core
INCLUDEPATH +=  $$PWD
DEPENDPATH +=   $$PWD
HEADERS +=      \
    $$PWD/aboutdialog.hpp \
    $$PWD/feedbackdialog.hpp \
    $$PWD/hangupdialog.hpp \
    $$PWD/logindialog.hpp \
    $$PWD/mainwindow.hpp \
    $$PWD/messageviewerdialog.hpp \
    $$PWD/noticedialog.hpp \
    $$PWD/popupinfomationdialog.hpp \
    $$PWD/settingsdialog.hpp

SOURCES +=      $$PWD/aboutdialog.cpp \
                $$PWD/mainwindow.cpp \
                $$PWD/logindialog.cpp \
                $$PWD/noticedialog.cpp \
                $$PWD/settingsdialog.cpp \
                $$PWD/messageviewerdialog.cpp \
                $$PWD/hangupdialog.cpp \
                $$PWD/feedbackdialog.cpp \
                $$PWD/popupinfomationdialog.cpp

FORMS +=        $$PWD/aboutdialog.ui \
                $$PWD/mainwindow.ui \
                $$PWD/logindialog.ui \
                $$PWD/noticedialog.ui \
                $$PWD/settingsdialog.ui \
                $$PWD/messageviewerdialog.ui \
                $$PWD/feedbackdialog.ui \
                $$PWD/hangupdialog.ui

win32 {
    LIBS += -lpsapi
}

