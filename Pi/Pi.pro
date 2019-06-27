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

PRE_TARGETDEPS = "C:/Projects/Px/Px/build-Px/release/libPx.a" \
                 "C:/Projects/Px/Px/build-Pd/release/Pd.exe" \
                 "C:/Projects/Px/Px/build-Pl/release/Pl.exe" \
                 "C:/Projects/Px/Px/build-Pv/release/Pv.exe"

SOURCES += \
        application/Context.cpp \
        compiler/Code.cpp \
        compiler/Compiler.cpp \
        compiler/Expr.cpp \
        compiler/Instruction.cpp \
        components/StringTable.cpp \
        main.cpp \
        nodes/AddrOfNode.cpp \
        nodes/IdNode.cpp \
        nodes/Node.cpp \
        nodes/StringNode.cpp \
        symbols/Sym.cpp \
        symbols/SymStack.cpp \
        visitors/AddrVisitor.cpp \
        visitors/AstPrinter.cpp \
        visitors/ByteWriter.cpp \
        visitors/PushVisitor.cpp \
        visitors/StoreVisitor.cpp \
        visitors/Visitor.cpp

HEADERS += \
    application/Context.h \
    compiler/Code.h \
    compiler/Compiler.h \
    compiler/Expr.h \
    compiler/Instruction.h \
    components/Function.h \
    components/Global.h \
    components/StringTable.h \
    nodes/AddrOfNode.h \
    nodes/IdNode.h \
    nodes/LiteralNode.h \
    nodes/Node.h \
    nodes/StringNode.h \
    symbols/Sym.h \
    symbols/SymStack.h \
    visitors/AddrVisitor.h \
    visitors/AstPrinter.h \
    visitors/ByteWriter.h \
    visitors/PushVisitor.h \
    visitors/StoreVisitor.h \
    visitors/Visitor.h

DISTFILES += \
    lib.pi \
    script.txt
