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
    ../examples/overloads.pc \
    ../lib/stdios.pc \
    ../lib/stdios.ph \
    ../lib/stdlib.ph \
    ../lib/stdlib.pi \
    ../lib/stdtest.pc \
    ../lib/stdtest.ph \
    ../unittests/addr_of_ref.pc \
    ../unittests/addr_of_ref_param.pc \
    ../unittests/break.pc \
    ../unittests/complex_ref_members.pc \
    ../unittests/const_method.pc \
    ../unittests/const_overload.pc \
    ../unittests/const_ref.pc \
    ../unittests/copy_construct_init.pc \
    ../unittests/copy_param.pc \
    ../unittests/copy_return.pc \
    ../unittests/destructors.pc \
    ../unittests/for_scope.pc \
    ../unittests/for_scope_count.pc \
    ../unittests/func_member_access.pc \
    ../unittests/func_ref.pc \
    ../unittests/generate_op_assign.pc \
    ../unittests/generated_new_copy_delete_funcs.pc \
    ../unittests/init_lists.pc \
    ../unittests/literal_to_ref.pc \
    ../unittests/must_return.pc \
    ../unittests/no_assign_gen.pc \
    ../unittests/non_prim_incdec.pc \
    ../unittests/non_prim_opeq.pc \
    ../unittests/nonprim_incdec.pc \
    ../unittests/operator_eq.pc \
    ../unittests/operator_eq_chain.pc \
    ../unittests/operator_test.pc \
    ../unittests/overload_test.pc \
    ../unittests/primitive_array.pc \
    ../unittests/primitive_incdec.pc \
    ../unittests/primitive_opeq.pc \
    ../unittests/private_copy.pc \
    ../unittests/ref_member.pc \
    ../unittests/ref_must_init.pc \
    ../unittests/ref_param.pc \
    ../unittests/ref_ptr.pc \
    ../unittests/ref_return_access.pc \
    ../unittests/ref_tests.pc \
    ../unittests/ref_to_ref_param.pc \
    ../unittests/ref_to_ref_param.pc \
    ../unittests/ref_to_value_param.pc \
    ../unittests/return_by_value.pc \
    ../unittests/return_ref.pc \
    ../unittests/return_ref.pc \
    ../unittests/simple_error_test.pc \
    ../unittests/simple_ref_assign.pc \
    ../unittests/simple_refs.pc \
    ../unittests/temp_access.pc \
    ../unittests/temp_construct.pc \
    ../unittests/temp_to_ref_param.pc \
    ../unittests/this_addr.pc \
    ../unittests/this_as_param.pc \
    ../unittests/this_ref.pc \
    ../workspace/pb.bat \
    ../workspace/script.pc \
    ../workspace/script.pi

HEADERS +=
