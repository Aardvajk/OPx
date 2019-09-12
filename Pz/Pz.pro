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
        main.cpp \
        nodes/BlockNode.cpp \
        nodes/IdNode.cpp \
        nodes/Node.cpp \
        operators/Operators.cpp \
        parser/CommonParser.cpp \
        parser/Parser.cpp \
        visitors/AstPrinter.cpp \
        visitors/DescVisitor.cpp \
        visitors/Visitor.cpp

HEADERS += \
    application/Context.h \
    nodes/BlockNode.h \
    nodes/IdNode.h \
    nodes/Node.h \
    operators/Operators.h \
    parser/CommonParser.h \
    parser/Parser.h \
    visitors/AstPrinter.h \
    visitors/DescVisitor.h \
    visitors/Visitor.h

DISTFILES += \
    script.pz
