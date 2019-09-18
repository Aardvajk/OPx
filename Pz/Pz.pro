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
        application/Pragmas.cpp \
        decorator/Decorator.cpp \
        decorator/ExprDecorator.cpp \
        decorator/FuncDecorator.cpp \
        decorator/TypeDecorator.cpp \
        decorator/VarDecorator.cpp \
        finaliser/Finaliser.cpp \
        generator/ExprGenerator.cpp \
        generator/FuncGenerator.cpp \
        generator/Generator.cpp \
        generator/LocalsGenerator.cpp \
        info/FuncInfo.cpp \
        lower/ExprLower.cpp \
        lower/FuncLower.cpp \
        lower/Lower.cpp \
        main.cpp \
        nodes/BlockNode.cpp \
        nodes/CallNode.cpp \
        nodes/ClassNode.cpp \
        nodes/ConstructNode.cpp \
        nodes/ExprNode.cpp \
        nodes/FuncNode.cpp \
        nodes/IdNode.cpp \
        nodes/LiteralNodes.cpp \
        nodes/NamespaceNode.cpp \
        nodes/Node.cpp \
        nodes/PragmaNode.cpp \
        nodes/ScopeNode.cpp \
        nodes/TypeNode.cpp \
        nodes/VarNode.cpp \
        operators/Operators.cpp \
        parser/CommonParser.cpp \
        parser/DeclarationParser.cpp \
        parser/ExprParser.cpp \
        parser/FuncParser.cpp \
        parser/Parser.cpp \
        parser/PragmaParser.cpp \
        parser/TypeParser.cpp \
        syms/Sym.cpp \
        syms/SymGuard.cpp \
        syms/SymPrinter.cpp \
        syms/SymTree.cpp \
        transform/ExprTransform.cpp \
        transform/FuncTransform.cpp \
        transform/Transform.cpp \
        types/DefaultTypes.cpp \
        types/LowerTypes.cpp \
        types/Type.cpp \
        types/TypeBuilder.cpp \
        types/TypeCache.cpp \
        types/TypeCompare.cpp \
        visitors/AstPrinter.cpp \
        visitors/DescVisitor.cpp \
        visitors/NameVisitors.cpp \
        visitors/QueryVisitors.cpp \
        visitors/SymFinder.cpp \
        visitors/SymScopeVisitor.cpp \
        visitors/TypeVisitor.cpp \
        visitors/Visitor.cpp

HEADERS += \
    application/Context.h \
    application/Pragmas.h \
    decorator/Decorator.h \
    decorator/ExprDecorator.h \
    decorator/FuncDecorator.h \
    decorator/TypeDecorator.h \
    decorator/VarDecorator.h \
    finaliser/Finaliser.h \
    generator/ExprGenerator.h \
    generator/FuncGenerator.h \
    generator/Generator.h \
    generator/LocalsGenerator.h \
    info/FuncInfo.h \
    lower/ExprLower.h \
    lower/FuncLower.h \
    lower/Lower.h \
    nodes/BlockNode.h \
    nodes/CallNode.h \
    nodes/ClassNode.h \
    nodes/ConstructNode.h \
    nodes/ExprNode.h \
    nodes/FuncNode.h \
    nodes/IdNode.h \
    nodes/LiteralNodes.h \
    nodes/NamespaceNode.h \
    nodes/Node.h \
    nodes/PragmaNode.h \
    nodes/ScopeNode.h \
    nodes/TypeNode.h \
    nodes/VarNode.h \
    operators/Operators.h \
    parser/CommonParser.h \
    parser/DeclarationParser.h \
    parser/ExprParser.h \
    parser/FuncParser.h \
    parser/Parser.h \
    parser/PragmaParser.h \
    parser/TypeParser.h \
    syms/Sym.h \
    syms/SymGuard.h \
    syms/SymPrinter.h \
    syms/SymTree.h \
    transform/ExprTransform.h \
    transform/FuncTransform.h \
    transform/Transform.h \
    types/DefaultTypes.h \
    types/LowerTypes.h \
    types/Type.h \
    types/TypeBuilder.h \
    types/TypeCache.h \
    types/TypeCompare.h \
    visitors/AstPrinter.h \
    visitors/DescVisitor.h \
    visitors/NameVisitors.h \
    visitors/QueryVisitors.h \
    visitors/SymFinder.h \
    visitors/SymScopeVisitor.h \
    visitors/TypeVisitor.h \
    visitors/Visitor.h

DISTFILES += \
    ../workspace/script.pz
