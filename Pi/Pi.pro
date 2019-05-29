TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= UNICODE

QMAKE_CXXFLAGS += -std=gnu++11

QMAKE_LFLAGS += -Wl,--exclude-all-symbols

QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas -Wno-comment -Wno-maybe-uninitialized -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-attributes

INCLUDEPATH += "C:/Projects/pcx" \
               "C:/Projects/Px/Px/Px"

LIBS += "C:/Projects/pcx/build-pcx/release/libpcx.a" \
        "C:/Projects/Px/Px/build-Px/release/libPx.a"

PRE_TARGETDEPS += "C:/Projects/Px/Px/build-Px/release/libPx.a"

SOURCES += \
        application/Context.cpp \
        application/StringTable.cpp \
        compiler/Compiler.cpp \
        compiler/FuncConstruct.cpp \
        compiler/Instruction.cpp \
        main.cpp \
        symbols/Sym.cpp \
        symbols/SymStack.cpp

HEADERS += \
    application/Context.h \
    application/Function.h \
    application/StringTable.h \
    compiler/Compiler.h \
    compiler/FuncConstruct.h \
    compiler/Instruction.h \
    symbols/Sym.h \
    symbols/SymStack.h

DISTFILES += \
    script.txt
