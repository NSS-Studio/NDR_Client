TEMPLATE = subdirs

SUBDIRS += $$PWD/auth/auth.pro \
           $$PWD/client/client.pro

CONFIG += ordered

QMAKE_CXXFLAGS += -std=c++17

CONFIG(debug, debug|release):{
    QMAKE_LFLAGS_RELEASE = /DEBUG
    CONFIG += exceptions
}

