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

PRE_TARGETDEPS += "C:/Projects/Px/Px/build-Px/release/libPx.a"

SOURCES += \
        application/Machine.cpp \
        components/FreeStore.cpp \
        components/Memory.cpp \
        main.cpp \
        operations/ConvertOps.cpp \
        operations/TestOps.cpp

HEADERS += \
    application/Machine.h \
    components/FreeStore.h \
    components/Memory.h \
    components/Registers.h \
    components/Stack.h \
    components/Vars.h \
    operations/CompareOps.h \
    operations/ConvertOps.h \
    operations/MathOps.h \
    operations/TestOps.h \
    operations/UnaryOps.h
