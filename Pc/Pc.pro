TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= UNICODE

QMAKE_CXXFLAGS += -std=gnu++11 \
                  -include "framework/Trace.h"

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
        convert/CommonConvert.cpp \
        convert/Convert.cpp \
        convert/ExprConvert.cpp \
        convert/FuncConvert.cpp \
        decorator/ArgDecorator.cpp \
        decorator/CommonDecorator.cpp \
        decorator/Decorator.cpp \
        decorator/DefaultMethods.cpp \
        decorator/ExprDecorator.cpp \
        decorator/FuncDecorator.cpp \
        decorator/TypeDecorator.cpp \
        decorator/VarDecorator.cpp \
        finaliser/Finaliser.cpp \
        generator/AddrGenerator.cpp \
        generator/ByteListGenerator.cpp \
        generator/CommonGenerator.cpp \
        generator/ExprGenerator.cpp \
        generator/FuncGenerator.cpp \
        generator/Generator.cpp \
        generator/GlobalsGenerator.cpp \
        generator/LocalsGenerator.cpp \
        info/FuncInfo.cpp \
        lower/ExprLower.cpp \
        lower/FuncLower.cpp \
        lower/Lower.cpp \
        main.cpp \
        nodes/AddrOfNode.cpp \
        nodes/AssignNode.cpp \
        nodes/BinaryNode.cpp \
        nodes/BlockNode.cpp \
        nodes/CallNode.cpp \
        nodes/ClassNode.cpp \
        nodes/CommaNode.cpp \
        nodes/ConstructNode.cpp \
        nodes/DerefNode.cpp \
        nodes/ExprNode.cpp \
        nodes/ForNode.cpp \
        nodes/FuncNode.cpp \
        nodes/IdNode.cpp \
        nodes/IfNode.cpp \
        nodes/IncDecNodes.cpp \
        nodes/InitNode.cpp \
        nodes/InlineVarNode.cpp \
        nodes/LiteralNodes.cpp \
        nodes/LogicalNode.cpp \
        nodes/NamespaceNode.cpp \
        nodes/Node.cpp \
        nodes/PragmaNode.cpp \
        nodes/ProxyCallNode.cpp \
        nodes/ReturnNode.cpp \
        nodes/ScopeNode.cpp \
        nodes/SubscriptNode.cpp \
        nodes/TernaryNode.cpp \
        nodes/TextNode.cpp \
        nodes/ThisNode.cpp \
        nodes/TypeCastNode.cpp \
        nodes/TypeNode.cpp \
        nodes/UnaryNode.cpp \
        nodes/UncheckedCastNode.cpp \
        nodes/VarNode.cpp \
        nodes/WhileNode.cpp \
        operators/CompareOperators.cpp \
        operators/MathOperators.cpp \
        operators/OperatorCallDecorate.cpp \
        operators/Operators.cpp \
        parser/CommonParser.cpp \
        parser/DeclarationParser.cpp \
        parser/ExprParser.cpp \
        parser/FuncParser.cpp \
        parser/IncludeParser.cpp \
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
        lower/LowerTypes.cpp \
        types/Type.cpp \
        types/TypeBuilder.cpp \
        types/TypeCache.cpp \
        types/TypeCompare.cpp \
        types/TypeConvert.cpp \
        types/TypeLookup.cpp \
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
    convert/CommonConvert.h \
    convert/Convert.h \
    convert/ExprConvert.h \
    convert/FuncConvert.h \
    decorator/ArgDecorator.h \
    decorator/CommonDecorator.h \
    decorator/Decorator.h \
    decorator/DefaultMethods.h \
    decorator/ExprDecorator.h \
    decorator/FuncDecorator.h \
    decorator/TypeDecorator.h \
    decorator/VarDecorator.h \
    finaliser/Finaliser.h \
    generator/AddrGenerator.h \
    generator/ByteListGenerator.h \
    generator/CommonGenerator.h \
    generator/ExprGenerator.h \
    generator/FuncGenerator.h \
    generator/Generator.h \
    generator/GlobalsGenerator.h \
    generator/LocalsGenerator.h \
    info/Access.h \
    info/FuncInfo.h \
    info/Qual.h \
    lower/ExprLower.h \
    lower/FuncLower.h \
    lower/Lower.h \
    nodes/AddrOfNode.h \
    nodes/AssignNode.h \
    nodes/BinaryNode.h \
    nodes/BlockNode.h \
    nodes/CallNode.h \
    nodes/ClassNode.h \
    nodes/CommaNode.h \
    nodes/ConstructNode.h \
    nodes/DerefNode.h \
    nodes/ExprNode.h \
    nodes/ForNode.h \
    nodes/FuncNode.h \
    nodes/IdNode.h \
    nodes/IfNode.h \
    nodes/IncDecNodes.h \
    nodes/InitNode.h \
    nodes/InlineVarNode.h \
    nodes/LiteralNodes.h \
    nodes/LogicalNode.h \
    nodes/NamespaceNode.h \
    nodes/Node.h \
    nodes/PragmaNode.h \
    nodes/ProxyCallNode.h \
    nodes/ReturnNode.h \
    nodes/ScopeNode.h \
    nodes/SubscriptNode.h \
    nodes/TernaryNode.h \
    nodes/TextNode.h \
    nodes/ThisNode.h \
    nodes/TypeCastNode.h \
    nodes/TypeNode.h \
    nodes/UnaryNode.h \
    nodes/UncheckedCastNode.h \
    nodes/VarNode.h \
    nodes/WhileNode.h \
    operators/CompareOperators.h \
    operators/MathOperators.h \
    operators/OperatorCallDecorate.h \
    operators/Operators.h \
    parser/CommonParser.h \
    parser/DeclarationParser.h \
    parser/ExprParser.h \
    parser/FuncParser.h \
    parser/IncludeParser.h \
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
    lower/LowerTypes.h \
    types/Type.h \
    types/TypeBuilder.h \
    types/TypeCache.h \
    types/TypeCompare.h \
    types/TypeConvert.h \
    types/TypeLookup.h \
    visitors/AstPrinter.h \
    visitors/DescVisitor.h \
    visitors/NameVisitors.h \
    visitors/QueryVisitors.h \
    visitors/SymFinder.h \
    visitors/SymScopeVisitor.h \
    visitors/TypeVisitor.h \
    visitors/Visitor.h

DISTFILES += \
    ../workspace/pb_c.bat \
    ../workspace/script.pc
