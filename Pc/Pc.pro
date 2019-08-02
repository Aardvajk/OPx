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
        compiler/IncludeConstructs.cpp \
        compiler/TypeConstructs.cpp \
        decorator/ClassDecorator.cpp \
        decorator/CommonDecorator.cpp \
        decorator/Decorator.cpp \
        decorator/ExprDecorator.cpp \
        decorator/FuncDecorator.cpp \
        generator/AddrGenerator.cpp \
        generator/ByteListGenerator.cpp \
        generator/ExprGenerator.cpp \
        generator/FuncGenerator.cpp \
        generator/Generator.cpp \
        generator/GlobalsGenerator.cpp \
        generator/LocalsGenerator.cpp \
        main.cpp \
        nodes/AddrOfNode.cpp \
        nodes/AssignNode.cpp \
        nodes/BinaryNode.cpp \
        nodes/BlockNode.cpp \
        nodes/CallNode.cpp \
        nodes/ClassNode.cpp \
        nodes/DerefNode.cpp \
        nodes/ExprNode.cpp \
        nodes/FuncNode.cpp \
        nodes/IdNode.cpp \
        nodes/LiteralNodes.cpp \
        nodes/NamespaceNode.cpp \
        nodes/Node.cpp \
        nodes/ReturnNode.cpp \
        nodes/ScopeNode.cpp \
        nodes/SubscriptNode.cpp \
        nodes/ThisNode.cpp \
        nodes/TypeNode.cpp \
        nodes/VarNode.cpp \
        nodes/WhileNode.cpp \
        operators/AddOperators.cpp \
        operators/Operators.cpp \
        syms/Sym.cpp \
        syms/SymGuard.cpp \
        syms/SymPrinter.cpp \
        syms/SymTree.cpp \
        transform/ExprTransformer.cpp \
        transform/FuncTransformer.cpp \
        transform/ThisCallTransformer.cpp \
        transform/Transformer.cpp \
        types/Type.cpp \
        types/TypeCache.cpp \
        types/TypeCompare.cpp \
        visitors/ArraySizeVisitor.cpp \
        visitors/AstPrinter.cpp \
        visitors/NameVisitors.cpp \
        visitors/SymFinder.cpp \
        types/TypeBuilder.cpp \
        visitors/SymScopeVisitor.cpp \
        visitors/TypeVisitor.cpp \
        visitors/Visitor.cpp

HEADERS += \
    application/Context.h \
    compiler/CodeConstructs.h \
    compiler/CommonConstructs.h \
    compiler/Compiler.h \
    compiler/DeclarationConstructs.h \
    compiler/ExprConstructs.h \
    compiler/IncludeConstructs.h \
    compiler/TypeConstructs.h \
    decorator/ClassDecorator.h \
    decorator/CommonDecorator.h \
    decorator/Decorator.h \
    decorator/ExprDecorator.h \
    decorator/FuncDecorator.h \
    generator/AddrGenerator.h \
    generator/ByteListGenerator.h \
    generator/ExprGenerator.h \
    generator/FuncGenerator.h \
    generator/Generator.h \
    generator/GlobalsGenerator.h \
    generator/LocalsGenerator.h \
    nodes/AddrOfNode.h \
    nodes/AssignNode.h \
    nodes/BinaryNode.h \
    nodes/BlockNode.h \
    nodes/CallNode.h \
    nodes/ClassNode.h \
    nodes/DerefNode.h \
    nodes/ExprNode.h \
    nodes/FuncNode.h \
    nodes/IdNode.h \
    nodes/LiteralNodes.h \
    nodes/NamespaceNode.h \
    nodes/Node.h \
    nodes/ReturnNode.h \
    nodes/ScopeNode.h \
    nodes/SubscriptNode.h \
    nodes/ThisNode.h \
    nodes/TypeNode.h \
    nodes/VarNode.h \
    nodes/WhileNode.h \
    operators/AddOperators.h \
    operators/Operators.h \
    syms/Sym.h \
    syms/SymGuard.h \
    syms/SymPrinter.h \
    syms/SymTree.h \
    transform/ExprTransformer.h \
    transform/FuncTransformer.h \
    transform/ThisCallTransformer.h \
    transform/Transformer.h \
    types/Type.h \
    types/TypeCache.h \
    types/TypeCompare.h \
    visitors/ArraySizeVisitor.h \
    visitors/AstPrinter.h \
    visitors/NameVisitors.h \
    visitors/SymFinder.h \
    types/TypeBuilder.h \
    visitors/SymScopeVisitor.h \
    visitors/TypeVisitor.h \
    visitors/Visitor.h

DISTFILES += \
    ../workspace/script.pc \
    ../workspace/stdlib.ph \
    ../workspace/store.pc
