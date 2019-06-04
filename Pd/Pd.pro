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

LIBS += "C:/Projects/pcx/build-pcx/release/libpcx.a" \
        "C:/Projects/Px/Px/build-Px/release/libPx.a"

SOURCES += \
        application/Context.cpp \
        application/Disassmble.cpp \
        application/Loader.cpp \
        application/Process.cpp \
        main.cpp

HEADERS += \
    application/Context.h \
    application/Disassmble.h \
    application/Loader.h \
    application/Process.h
