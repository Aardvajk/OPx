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

PRE_TARGETDEPS = "C:/Projects/Px/Px/build-Px/release/libPx.a" \
                 "C:/Projects/Px/Px/build-Pc/release/Pc.exe" \
                 "C:/Projects/Px/Px/build-Pi/release/Pi.exe" \
                 "C:/Projects/Px/Px/build-Pd/release/Pd.exe" \
                 "C:/Projects/Px/Px/build-Pl/release/Pl.exe" \
                 "C:/Projects/Px/Px/build-Pv/release/Pv.exe"

SOURCES += \
        main.cpp

DISTFILES += \
    ../lib/stdlib.ph \
    ../lib/stdlib.pi \
    ../lib/stdtest.pc \
    ../lib/stdtest.ph \
    ../unittests/addr_of_ref.pc \
    ../unittests/complex_ref_members.pc \
    ../unittests/copy_construct_init.pc \
    ../unittests/destructors.pc \
    ../unittests/init_lists.pc \
    ../unittests/ref_member.pc \
    ../unittests/ref_param.pc \
    ../unittests/ref_to_ref_param.pc \
    ../unittests/return_ref.pc \
    ../unittests/simple_ref_assign.pc \
    ../unittests/simple_refs.pc \
    ../unittests/temp_construct.pc \
    ../unittests/this_ref.pc \
    ../workspace/pb.bat \
    ../workspace/script.pc \
    ../workspace/script.pi

HEADERS +=
