# Bazel BUILD file for ISL.
#

licenses(["notice"])

exports_files(["LICENSE.TXT"])

load(
    "@org_tensorflow//third_party/isl:isl.bzl",
    "expand_isl_cmake_vars",
    "isl_all_cmake_vars",
)

package(default_visibility = ["//visibility:public"])

# Isl - Config file.
expand_isl_cmake_vars(
    name = "isl_config",
    src = "isl-0.18/isl_config.h.in",
    cmake_vars = isl_all_cmake_vars,
    dst = "isl-0.18/isl_config.h",
)

expand_isl_cmake_vars(
    name = "isl_srcdir",
    src = "isl-0.18/isl_srcdir.c.in",
    cmake_vars = isl_all_cmake_vars,
    dst = "isl-0.18/isl_srcdir.c",
)

genrule(
    name = "configure_stdint_h",
    outs = ["isl-0.18/include/isl/stdint.h"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "#include <stdint.h>",
        "",
        "EOF",
    ]),
)

genrule(
    name = "gen_gitversion_h",
    srcs = ["isl-0.18/GIT_HEAD_ID"],
    outs = ["isl-0.18/gitversion.h"],
    cmd = "echo '#define GIT_HEAD_ID \"isl-0.18-1047-g4a20ef8\"' | tee $@",
)



cc_library(
    name = "isl_test_inputs",
    srcs = glob([
        "isl-0.18/test_inputs/*.c",
        "isl-0.18/test_inputs/*.cpp",
    ]),
    hdrs = glob([
        "isl-0.18/test_inputs/*.h",
    ]),
)

cc_library(
    name = "imath",
    srcs = glob([
        "isl-0.18/imath/*.c",
        "isl-0.18/imath/*.cpp",
    ]),
    hdrs = glob([
        "isl-0.18/imath/*.h",
    ]),
)

cc_library(
    name = "imath_wrap",
    srcs = glob([
        "isl-0.18/imath_wrap/*.c",
        "isl-0.18/imath_wrap/*.cpp",
    ]),
    hdrs = glob([
        "isl-0.18/imath_wrap/*.h",
        "isl-0.18/imath/*.h",
        "isl-0.18/imath/*.c",
    ]),
)

cc_library(
    name = "isl",
    srcs = glob([
	    "isl-0.18/*.c",
	    "isl-0.18/*.cpp",
        ], exclude = [
	    "isl-0.18/*templ.c",
	    "isl-0.18/isl_pw_hash.c",
	    "isl-0.18/isl_multi_apply_union_set.c",
	    "isl-0.18/isl_multi_gist.c",
	    "isl-0.18/isl_pw_union_opt.c",
	    "isl-0.18/isl_multi_cmp.c",
	    "isl-0.18/isl_multi_templ.c",
            #"isl-0.18/isl_multi_dims.c",
            #"isl-0.18/isl_multi_align_set.c",
	    "isl-0.18/print_templ.c",
	    "isl-0.18/isl_union_neg.c",
	    "isl-0.18/isl_multi_coalesce.c",
	    "isl-0.18/isl_multi_apply_set.c",
	    #"isl-0.18/isl_int_sioimath.c",
	    "isl-0.18/isl_multi_intersect.c",
	    "isl-0.18/isl_val_sioimath.c",
	    "isl-0.18/print_templ_yaml.c",
	    "isl-0.18/isl_ast_int.c",
	    "isl-0.18/isl_val_gmp.c",
	    "isl-0.18/isl_union_single.c",
	    "isl-0.18/isl_multi_hash.c",
	    "isl-0.18/mp_get_memory_functions.c",
	    "isl-0.18/isl_union_eval.c",
	    "isl-0.18/isl_multi_floor.c",
	    "isl-0.18/isl_gmp.c",
	    "isl-0.18/isl_union_multi.c",
	    "isl-0.18/isl_test_int.c",
	    "isl-0.18/isl_test_imath.c",
	    "isl-0.18/bound.c",
	    "isl-0.18/cat.c",
	    "isl-0.18/closure.c",
	    "isl-0.18/codegen.c",
	    "isl-0.18/isl_test.c",
	    "isl-0.18/isl_test_imath.c",
            "isl-0.18/pip.c",
	    "isl-0.18/polyhedron_detect_equalities.c",
	    "isl-0.18/polyhedron_minimize.c",
	    "isl-0.18/polyhedron_sample.c",
	    "isl-0.18/polytope_scan.c",
	    "isl-0.18/schedule.c",
	]),
    hdrs = glob([
        "isl-0.18/isl_pw_hash.c",
        "isl-0.18/isl_multi_gist.c",
        "isl-0.18/isl_pw_union_opt.c",
        "isl-0.18/read_in_string_templ.c",
        "isl-0.18/isl_multi_cmp.c",
        "isl-0.18/isl_multi_templ.c",
        "isl-0.18/isl_tab_lexopt_templ.c",
        "isl-0.18/print_templ.c",
        "isl-0.18/isl_multi_apply_templ.c",
        "isl-0.18/isl_union_neg.c",
        "isl-0.18/*.h",
        "isl-0.18/*templ.c",
	"isl-0.18/isl_multi_coalesce.c",
	"isl-0.18/isl_multi_apply_set.c",
	"isl-0.18/isl_int_sioimath.c",
	"isl-0.18/isl_multi_intersect.c",
	"isl-0.18/isl_val_sioimath.c",
	"isl-0.18/print_templ_yaml.c",
	"isl-0.18/isl_val_gmp.c",
	"isl-0.18/isl_union_single.c",
	"isl-0.18/isl_multi_hash.c",
	"isl-0.18/mp_get_memory_functions.c",
	"isl-0.18/isl_union_eval.c",
	"isl-0.18/isl_multi_floor.c",
	"isl-0.18/isl_union_multi.c",
	"isl-0.18/isl_test_int.c",
	"isl-0.18/isl_test_imath.c",
	"isl-0.18/bound.c",
	"isl-0.18/cat.c",
	"isl-0.18/closure.c",
	"isl-0.18/codegen.c",
	"isl-0.18/isl_test.c",
	"isl-0.18/isl_test_imath.c",
	"isl-0.18/isl_test_int.c",
	"isl-0.18/pip.c",
	"isl-0.18/polyhedron_detect_equalities.c",
	"isl-0.18/polyhedron_minimize.c",
	"isl-0.18/polyhedron_sample.c",
	"isl-0.18/polytope_scan.c",
	"isl-0.18/schedule.c",
        "isl-0.18/isl_int_imath.h",
        "isl-0.18/include/**/*.h",
    ]) + [
       "isl-0.18/isl_config.h",
       "isl-0.18/isl_srcdir.c",
       "isl-0.18/gitversion.h",
       "isl-0.18/include/isl/stdint.h"],
    includes = [
	"isl-0.18/include",
	#"tools/polly/lib/External/isl/",
	"isl-0.18/",
        "isl-0.18/imath",
    ],
    deps = [
        ":imath",
        ":imath_wrap",
    ],
)
