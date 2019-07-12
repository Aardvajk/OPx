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

PRE_TARGETDEPS = "C:/Projects/Px/Px/build-Px/release/libPx.a"

SOURCES += \
        application/Context.cpp \
        compiler/CodeConstructs.cpp \
        compiler/CommonConstructs.cpp \
        compiler/Compiler.cpp \
        compiler/DeclarationConstructs.cpp \
        compiler/ExprConstructs.cpp \
        compiler/TypeConstructs.cpp \
        generator/Generator.cpp \
        main.cpp \
        nodes/BlockNode.cpp \
        nodes/CallNode.cpp \
        nodes/ClassNode.cpp \
        nodes/DotNode.cpp \
        nodes/ExprNode.cpp \
        nodes/FuncNode.cpp \
        nodes/IdNode.cpp \
        nodes/LiteralNodes.cpp \
        nodes/NamespaceNode.cpp \
        nodes/Node.cpp \
        nodes/TypeNode.cpp \
        nodes/VarNode.cpp \
        syms/Sym.cpp \
        syms/SymGuard.cpp \
        syms/SymPrinter.cpp \
        syms/SymTree.cpp \
        visitors/AstPrinter.cpp \
        visitors/NameVisitors.cpp \
        visitors/SymFinder.cpp \
        visitors/Visitor.cpp

HEADERS += \
    application/Context.h \
    compiler/CodeConstructs.h \
    compiler/CommonConstructs.h \
    compiler/Compiler.h \
    compiler/DeclarationConstructs.h \
    compiler/ExprConstructs.h \
    compiler/TypeConstructs.h \
    generator/Generator.h \
    nodes/BlockNode.h \
    nodes/CallNode.h \
    nodes/ClassNode.h \
    nodes/DotNode.h \
    nodes/ExprNode.h \
    nodes/FuncNode.h \
    nodes/IdNode.h \
    nodes/LiteralNodes.h \
    nodes/NamespaceNode.h \
    nodes/Node.h \
    nodes/TypeNode.h \
    nodes/VarNode.h \
    syms/Sym.h \
    syms/SymGuard.h \
    syms/SymPrinter.h \
    syms/SymTree.h \
    visitors/AstPrinter.h \
    visitors/NameVisitors.h \
    visitors/SymFinder.h \
    visitors/Visitor.h

DISTFILES += \
    script.pc
