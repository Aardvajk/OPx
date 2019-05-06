TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=gnu++11

QMAKE_LFLAGS += -Wl,--exclude-all-symbols

QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas -Wno-comment -Wno-maybe-uninitialized -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-attributes

INCLUDEPATH += "C:/Projects/pcx"

LIBS += "C:/Projects/pcx/build-pcx/release/libpcx.a"

SOURCES += main.cpp \
    scanner/Source.cpp \
    scanner/Token.cpp \
    scanner/Lexer.cpp \
    application/Context.cpp

HEADERS += \
    scanner/Source.h \
    scanner/Token.h \
    scanner/Lexer.h \
    application/Error.h \
    scanner/Location.h \
    application/Context.h

DISTFILES += \
    script.txt
