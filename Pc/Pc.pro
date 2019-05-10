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
    scanner/Scanner.cpp \
    application/Context.cpp \
    scanner/SourceList.cpp \
    nodes/Node.cpp \
    nodes/BlockNode.cpp \
    nodes/IdNode.cpp \
    visitors/AstPrinter.cpp \
    nodes/ClassNode.cpp \
    nodes/DotNode.cpp \
    compiler/Compiler.cpp \
    symbols/Sym.cpp \
    symbols/SymTree.cpp \
    symbols/SymGuard.cpp \
    visitors/Visitor.cpp \
    visitors/SymFinder.cpp \
    visitors/IdResolver.cpp

HEADERS += \
    scanner/Source.h \
    scanner/Token.h \
    scanner/Lexer.h \
    application/Error.h \
    scanner/Location.h \
    scanner/Scanner.h \
    application/Context.h \
    scanner/SourceList.h \
    nodes/Node.h \
    nodes/BlockNode.h \
    nodes/IdNode.h \
    visitors/Visitor.h \
    visitors/AstPrinter.h \
    nodes/ClassNode.h \
    nodes/DotNode.h \
    compiler/Compiler.h \
    symbols/Sym.h \
    symbols/SymTree.h \
    symbols/SymGuard.h \
    visitors/SymFinder.h \
    visitors/IdResolver.h

DISTFILES += \
    script.txt
