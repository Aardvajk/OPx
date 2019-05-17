TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= UNICODE

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
    visitors/NameVisitors.cpp \
    nodes/NamespaceNode.cpp \
    nodes/GlobalNode.cpp \
    symbols/SymPrinter.cpp \
    compiler/CommonConstructs.cpp \
    compiler/TestConstructs.cpp \
    tests/Tests.cpp \
    compiler/DeclarationConstructs.cpp \
    types/Type.cpp \
    compiler/TypeConstructs.cpp \
    compiler/VarConstructs.cpp \
    types/TypeCache.cpp \
    types/TypeCompare.cpp \
    nodes/VarNode.cpp \
    nodes/FuncNode.cpp

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
    visitors/NameVisitors.h \
    nodes/NamespaceNode.h \
    nodes/GlobalNode.h \
    symbols/SymPrinter.h \
    compiler/CommonConstructs.h \
    compiler/TestConstructs.h \
    tests/Tests.h \
    compiler/DeclarationConstructs.h \
    types/Type.h \
    compiler/TypeConstructs.h \
    compiler/VarConstructs.h \
    types/TypeCache.h \
    types/TypeCompare.h \
    nodes/VarNode.h \
    nodes/FuncNode.h

DISTFILES += \
    script.txt \
    tests/scripts/lookup.txt \
    tests/scripts/aliases.txt \
    tests/scripts/lookupfails.txt \
    tests/scripts/ambiguous.txt
