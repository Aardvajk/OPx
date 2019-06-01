TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= UNICODE

QMAKE_CXXFLAGS += -std=gnu++11

QMAKE_LFLAGS += -Wl,--exclude-all-symbols

QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas -Wno-comment -Wno-maybe-uninitialized -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-attributes
QMAKE_CXXFLAGS_WARN_OFF += Wunused-parameter

INCLUDEPATH += "C:/Projects/pcx" \
               "C:/Projects/Px/Px/Px"

SOURCES += \
        main.cpp
