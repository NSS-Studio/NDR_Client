TEMPLATE = subdirs

SUBDIRS += $$PWD/client/client.pro #$$PWD/auth/auth.pro \


CONFIG += ordered


CONFIG(debug, debug|release):{
    QMAKE_LFLAGS_RELEASE = /DEBUG
}
