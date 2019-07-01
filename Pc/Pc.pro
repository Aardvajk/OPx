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

SOURCES += main.cpp \
    application/Context.cpp \
    compiler/CodeConstructs.cpp \
    compiler/FuncConstructs.cpp \
    generator/ByteLister.cpp \
    generator/CodeGenerator.cpp \
    generator/ExprGenerator.cpp \
    generator/Generator.cpp \
    generator/LocalsGenerator.cpp \
    nodes/CallNode.cpp \
    nodes/CharLiteralNode.cpp \
    nodes/ExprNode.cpp \
    nodes/FuncDecNode.cpp \
    nodes/Node.cpp \
    nodes/BlockNode.cpp \
    nodes/IdNode.cpp \
    nodes/ReturnNode.cpp \
    nodes/ScopeNode.cpp \
    visitors/AstPrinter.cpp \
    nodes/ClassNode.cpp \
    nodes/DotNode.cpp \
    compiler/Compiler.cpp \
    symbols/Sym.cpp \
    symbols/SymTree.cpp \
    symbols/SymGuard.cpp \
    visitors/LookupVisitor.cpp \
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
    nodes/FuncNode.cpp \
    compiler/Expr.cpp \
    nodes/IntLiteralNode.cpp \
    visitors/TypeVisitor.cpp

HEADERS += \
    application/Context.h \
    compiler/CodeConstructs.h \
    compiler/FuncConstructs.h \
    generator/ByteLister.h \
    generator/CodeGenerator.h \
    generator/ExprGenerator.h \
    generator/Generator.h \
    generator/LocalsGenerator.h \
    nodes/CallNode.h \
    nodes/CharLiteralNode.h \
    nodes/ExprNode.h \
    nodes/FuncDecNode.h \
    nodes/Node.h \
    nodes/BlockNode.h \
    nodes/IdNode.h \
    nodes/ReturnNode.h \
    nodes/ScopeNode.h \
    symbols/SymResult.h \
    visitors/LookupVisitor.h \
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
    nodes/FuncNode.h \
    compiler/Expr.h \
    nodes/IntLiteralNode.h \
    visitors/TypeVisitor.h

DISTFILES += \
    tests/scripts/locallookup.txt \
    tests/scripts/lookup.txt \
    tests/scripts/funclookup.txt \
    tests/scripts/classlookup.txt \
    tests/scripts/lookupfwd.txt
