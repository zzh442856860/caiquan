TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    mainserver.cpp

INCLUDEPATH += -I /usr/local/boost_1_55_0/

LIBS += -lmuduo_net -lmuduo_base  \
        -lpthread \

HEADERS += \
    mainserver.h
