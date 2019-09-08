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
        compiler/CommonConstructs.cpp \
        compiler/Compiler.cpp \
        compiler/DeclarationConstructs.cpp \
        compiler/ExprConstructs.cpp \
        compiler/FuncConstructs.cpp \
        compiler/IncludeConstructs.cpp \
        compiler/PragmaConstructs.cpp \
        compiler/TypeConstructs.cpp \
        decorator/ClassDecorator.cpp \
        decorator/ClassMethodDecorator.cpp \
        decorator/CommonDecorator.cpp \
        decorator/Decorator.cpp \
        decorator/ExprDecorator.cpp \
        decorator/FuncDecorator.cpp \
        generator/AddrGenerator.cpp \
        generator/ByteListGenerator.cpp \
        generator/CommonGenerator.cpp \
        generator/ExprGenerator.cpp \
        generator/FuncGenerator.cpp \
        generator/Generator.cpp \
        generator/GlobalsGenerator.cpp \
        generator/LocalsGenerator.cpp \
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
        nodes/DerefNode.cpp \
        nodes/ExprNode.cpp \
        nodes/ForNode.cpp \
        nodes/FuncNode.cpp \
        nodes/IdNode.cpp \
        nodes/IfNode.cpp \
        nodes/IncDecNode.cpp \
        nodes/InitNode.cpp \
        nodes/LiteralNodes.cpp \
        nodes/LogicalNode.cpp \
        nodes/NamespaceNode.cpp \
        nodes/Node.cpp \
        nodes/OpEqNode.cpp \
        nodes/PragmaNode.cpp \
        nodes/PrimitiveCastNode.cpp \
        nodes/ReturnNode.cpp \
        nodes/ScopeNode.cpp \
        nodes/SubscriptNode.cpp \
        nodes/ThisNode.cpp \
        nodes/TypeNode.cpp \
        nodes/UnaryNode.cpp \
        nodes/VarNode.cpp \
        nodes/WhileNode.cpp \
        operators/CompareOperators.cpp \
        operators/MathOperators.cpp \
        operators/Operators.cpp \
        optimise/EllideConstructorCopies.cpp \
        syms/Sym.cpp \
        syms/SymGuard.cpp \
        syms/SymPrinter.cpp \
        syms/SymTree.cpp \
        transform/ExprTransformer.cpp \
        transform/FuncTransformer.cpp \
        transform/InitMapBuilder.cpp \
        transform/ThisCallTransformer.cpp \
        transform/Transformer.cpp \
        types/Type.cpp \
        types/TypeCache.cpp \
        types/TypeCompare.cpp \
        types/TypeLookup.cpp \
        visitors/ArraySizeVisitor.cpp \
        visitors/AstPrinter.cpp \
        visitors/ConstChainCheck.cpp \
        visitors/NameVisitors.cpp \
        visitors/SymFinder.cpp \
        types/TypeBuilder.cpp \
        visitors/SymScopeVisitor.cpp \
        visitors/TakesAddrVisitor.cpp \
        visitors/TypeVisitor.cpp \
        visitors/Visitor.cpp

HEADERS += \
    application/Context.h \
    application/Pragmas.h \
    compiler/CommonConstructs.h \
    compiler/Compiler.h \
    compiler/DeclarationConstructs.h \
    compiler/ExprConstructs.h \
    compiler/FuncConstructs.h \
    compiler/IncludeConstructs.h \
    compiler/PragmaConstructs.h \
    compiler/TypeConstructs.h \
    decorator/ClassDecorator.h \
    decorator/ClassMethodDecorator.h \
    decorator/CommonDecorator.h \
    decorator/Decorator.h \
    decorator/ExprDecorator.h \
    decorator/FuncDecorator.h \
    generator/AddrGenerator.h \
    generator/ByteListGenerator.h \
    generator/CommonGenerator.h \
    generator/ExprGenerator.h \
    generator/FuncGenerator.h \
    generator/Generator.h \
    generator/GlobalsGenerator.h \
    generator/LocalsGenerator.h \
    lower/ExprLower.h \
    lower/FuncLower.h \
    lower/Lower.h \
    nodes/AddrOfNode.h \
    nodes/AssignNode.h \
    nodes/BinaryNode.h \
    nodes/BlockNode.h \
    nodes/CallNode.h \
    nodes/ClassNode.h \
    nodes/DerefNode.h \
    nodes/ExprNode.h \
    nodes/ForNode.h \
    nodes/FuncNode.h \
    nodes/IdNode.h \
    nodes/IfNode.h \
    nodes/IncDecNode.h \
    nodes/InitNode.h \
    nodes/LiteralNodes.h \
    nodes/LogicalNode.h \
    nodes/NamespaceNode.h \
    nodes/Node.h \
    nodes/OpEqNode.h \
    nodes/PragmaNode.h \
    nodes/PrimitiveCastNode.h \
    nodes/ReturnNode.h \
    nodes/ScopeNode.h \
    nodes/SubscriptNode.h \
    nodes/ThisNode.h \
    nodes/TypeNode.h \
    nodes/UnaryNode.h \
    nodes/VarNode.h \
    nodes/WhileNode.h \
    operators/CompareOperators.h \
    operators/MathOperators.h \
    operators/Operators.h \
    optimise/EllideConstructorCopies.h \
    syms/Sym.h \
    syms/SymGuard.h \
    syms/SymPrinter.h \
    syms/SymTree.h \
    transform/ExprTransformer.h \
    transform/FuncTransformer.h \
    transform/InitMapBuilder.h \
    transform/ThisCallTransformer.h \
    transform/Transformer.h \
    types/Type.h \
    types/TypeCache.h \
    types/TypeCompare.h \
    types/TypeLookup.h \
    visitors/ArraySizeVisitor.h \
    visitors/AstPrinter.h \
    visitors/ConstChainCheck.h \
    visitors/NameVisitors.h \
    visitors/SymFinder.h \
    types/TypeBuilder.h \
    visitors/SymScopeVisitor.h \
    visitors/TakesAddrVisitor.h \
    visitors/TypeVisitor.h \
    visitors/Visitor.h

DISTFILES += \
    ../lib/stdios.pc \
    ../lib/stdios.ph \
    ../workspace/notes.txt \
    ../workspace/script.pc \
    ../workspace/script.pi \
    ../workspace/store.pc \
    ../workspace/string.pc \
    ../workspace/string.ph
