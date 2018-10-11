TEMPLATE = subdirs

SUBDIRS += $$PWD/client/client.pro

CONFIG += ordered

CONFIG(debug, debug|release):{
    QMAKE_LFLAGS_RELEASE = /DEBUG
}
