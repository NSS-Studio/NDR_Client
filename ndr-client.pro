TEMPLATE = subdirs

SUBDIRS += $$PWD/auth/auth.pro \
           $$PWD/client/client.pro

CONFIG += ordered

QT += c++14


CONFIG(debug, debug|release):{
    CONFIG += exceptions
}

