QT +=	widgets gui xml core
INCLUDEPATH +=  $$PWD
DEPENDPATH +=   $$PWD
HEADERS +=      $$PWD/aboutdialog.h \
                $$PWD/mainwindow.h \
                $$PWD/logindialog.h \
                $$PWD/noticedialog.h \
                $$PWD/settingsdialog.h \
                $$PWD/messageviewerdialog.h \
                $$PWD/hangupdialog.h \
                $$PWD/feedbackdialog.h

SOURCES +=      $$PWD/aboutdialog.cpp \
                $$PWD/mainwindow.cpp \
                $$PWD/logindialog.cpp \
                $$PWD/noticedialog.cpp \
                $$PWD/settingsdialog.cpp \
                $$PWD/messageviewerdialog.cpp \
                $$PWD/hangupdialog.cpp \
                $$PWD/feedbackdialog.cpp

FORMS +=        $$PWD/aboutdialog.ui \
                $$PWD/mainwindow.ui \
                $$PWD/logindialog.ui \
                $$PWD/noticedialog.ui \
                $$PWD/settingsdialog.ui \
                $$PWD/messageviewerdialog.ui \
                $$PWD/feedbackdialog.ui \
                $$PWD/hangupdialog.ui

#千万不用$$PWD 要不然auth相对路径会错误
#这样解决了一个编译依赖问题
INCLUDEPATH +=  ../auth/
LIBS +=         ../auth/libauth.a
