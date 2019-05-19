TEMPLATE = lib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += staticlib

DEFINES -= UNICODE

QMAKE_CXXFLAGS += -std=gnu++11

QMAKE_LFLAGS += -Wl,--exclude-all-symbols

QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas -Wno-comment -Wno-maybe-uninitialized -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-attributes

INCLUDEPATH += "C:/Projects/pcx"

LIBS += "C:/Projects/pcx/build-pcx/release/libpcx.a"

SOURCES += \
    scanner/Lexer.cpp \
    scanner/Scanner.cpp \
    scanner/Source.cpp \
    scanner/SourceList.cpp \
    scanner/Token.cpp

HEADERS += \
    error/Error.h \
    scanner/Lexer.h \
    scanner/Location.h \
    scanner/Scanner.h \
    scanner/Source.h \
    scanner/SourceList.h \
    scanner/Token.h
