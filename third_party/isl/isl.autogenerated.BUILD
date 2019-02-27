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
    src = "isl-0.20/isl_config.h.in",
    cmake_vars = isl_all_cmake_vars,
    dst = "isl-0.20/isl_config.h",
)

expand_isl_cmake_vars(
    name = "isl_srcdir",
    src = "isl-0.20/isl_srcdir.c.in",
    cmake_vars = isl_all_cmake_vars,
    dst = "isl-0.20/isl_srcdir.c",
)

genrule(
    name = "configure_stdint_h",
    outs = ["isl-0.20/include/isl/stdint.h"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "#include <stdint.h>",
        "",
        "EOF",
    ]),
)

genrule(
    name = "correct_imath_wrap_h",
    srcs = ["isl-0.20/imath_wrap/wrap.h"],
    outs = [],
    cmd = "sed 's/#endif/#define impz_fdiv_q_ui\tisl_impz_fdiv_q_ui\n#endif' $@",
)

genrule(
    name = "gen_gitversion_h",
    srcs = ["isl-0.20/GIT_HEAD_ID"],
    outs = ["isl-0.20/gitversion.h"],
    cmd = "echo '#define GIT_HEAD_ID \"isl-0.20-1047-g4a20ef8\"' | tee $@",
)



cc_library(
    name = "isl_test_inputs",
    srcs = glob([
        "isl-0.20/test_inputs/*.c",
        "isl-0.20/test_inputs/*.cpp",
    ]),
    hdrs = glob([
        "isl-0.20/test_inputs/*.h",
    ]),
)

cc_library(
    name = "imath",
    srcs = glob([
        "isl-0.20/imath/*.c",
        "isl-0.20/imath/*.cpp",
    ]),
    hdrs = glob([
        "isl-0.20/imath/*.h",
    ]),
)

cc_library(
    name = "imath_wrap",
    srcs = glob([
        "isl-0.20/imath_wrap/*.c",
        "isl-0.20/imath_wrap/*.cpp",
    ]),
    hdrs = glob([
        "isl-0.20/imath_wrap/*.h",
        "isl-0.20/imath/*.h",
        "isl-0.20/imath/*.c",
    ]),
)

cc_library(
    name = "isl",
    srcs = glob([
	    "isl-0.20/*.c",
	    "isl-0.20/*.cpp",
        ], exclude = [
	    "isl-0.20/*templ.c",
	    "isl-0.20/isl_pw_hash.c",
	    "isl-0.20/isl_multi_apply_union_set.c",
	    "isl-0.20/isl_multi_gist.c",
	    "isl-0.20/isl_pw_union_opt.c",
	    "isl-0.20/isl_multi_cmp.c",
	    "isl-0.20/isl_multi_templ.c",
	    "isl-0.20/isl_multi_dims.c",
	    "isl-0.20/isl_multi_align_set.c",
	    "isl-0.20/isl_multi_align_union_set.c",
            "isl-0.20/isl_multi_union_pw_aff_explicit_domain.c",
	    "isl-0.20/isl_multi_no_explicit_domain.c",
	    "isl-0.20/extract_key.c",
	    "isl-0.20/isl_pw_eval.c",
	    "isl-0.20/schedule_cmp.c",
 	    "isl-0.20/flow_cmp.c",
	    "isl-0.20/isl_multi_explicit_domain.c",
	    "isl-0.20/isl_multi_pw_aff_explicit_domain.c",
	    "isl-0.20/print_templ.c",
	    "isl-0.20/isl_union_neg.c",
	    "isl-0.20/isl_multi_coalesce.c",
	    "isl-0.20/isl_multi_apply_set.c",
	    #"isl-0.20/isl_int_sioimath.c",
	    "isl-0.20/isl_multi_intersect.c",
	    "isl-0.20/isl_val_sioimath.c",
	    "isl-0.20/print_templ_yaml.c",
	    "isl-0.20/isl_ast_int.c",
	    "isl-0.20/isl_val_gmp.c",
	    "isl-0.20/isl_union_single.c",
	    "isl-0.20/isl_multi_hash.c",
	    "isl-0.20/mp_get_memory_functions.c",
	    "isl-0.20/isl_union_eval.c",
	    "isl-0.20/isl_multi_floor.c",
	    "isl-0.20/isl_gmp.c",
	    "isl-0.20/isl_union_multi.c",
	    "isl-0.20/isl_test_int.c",
	    "isl-0.20/isl_test_imath.c",
	    "isl-0.20/bound.c",
	    "isl-0.20/cat.c",
	    "isl-0.20/closure.c",
	    "isl-0.20/codegen.c",
	    "isl-0.20/isl_test.c",
	    "isl-0.20/isl_test_imath.c",
            "isl-0.20/pip.c",
	    "isl-0.20/polyhedron_detect_equalities.c",
	    "isl-0.20/polyhedron_minimize.c",
	    "isl-0.20/polyhedron_sample.c",
	    "isl-0.20/polytope_scan.c",
	    "isl-0.20/schedule.c",
	]),
    hdrs = glob([
        "isl-0.20/isl_pw_hash.c",
        "isl-0.20/isl_multi_gist.c",
        "isl-0.20/isl_pw_union_opt.c",
        "isl-0.20/read_in_string_templ.c",
        "isl-0.20/isl_multi_cmp.c",
        "isl-0.20/isl_multi_templ.c",
        "isl-0.20/isl_tab_lexopt_templ.c",
        "isl-0.20/print_templ.c",
        "isl-0.20/isl_multi_apply_templ.c",
        "isl-0.20/isl_multi_align_union_set.c",
        "isl-0.20/isl_union_neg.c",
        "isl-0.20/*.h",
        "isl-0.20/*templ.c",
        "isl-0.20/isl_multi_union_pw_aff_explicit_domain.c",
        "isl-0.20/isl_multi_no_explicit_domain.c",
        "isl-0.20/extract_key.c",
        "isl-0.20/isl_pw_eval.c",
        "isl-0.20/isl_multi_explicit_domain.c",
        "isl-0.20/isl_multi_pw_aff_explicit_domain.c",
	"isl-0.20/isl_multi_coalesce.c",
	"isl-0.20/isl_multi_apply_set.c",
	"isl-0.20/isl_int_sioimath.c",
	"isl-0.20/isl_multi_intersect.c",
	"isl-0.20/isl_val_sioimath.c",
        #"isl-0.20/schedule_cmp.c",
        #"isl-0.20/flow_cmp.c",
	"isl-0.20/print_templ_yaml.c",
	"isl-0.20/isl_val_gmp.c",
	"isl-0.20/isl_union_single.c",
	"isl-0.20/isl_multi_hash.c",
	"isl-0.20/mp_get_memory_functions.c",
	"isl-0.20/isl_union_eval.c",
	"isl-0.20/isl_multi_floor.c",
	"isl-0.20/isl_union_multi.c",
	"isl-0.20/isl_test_int.c",
	"isl-0.20/isl_test_imath.c",
	"isl-0.20/bound.c",
	"isl-0.20/cat.c",
	"isl-0.20/closure.c",
	"isl-0.20/codegen.c",
	"isl-0.20/isl_test.c",
	"isl-0.20/isl_test_imath.c",
	"isl-0.20/isl_test_int.c",
	"isl-0.20/pip.c",
	"isl-0.20/polyhedron_detect_equalities.c",
	"isl-0.20/polyhedron_minimize.c",
	"isl-0.20/polyhedron_sample.c",
	"isl-0.20/polytope_scan.c",
	"isl-0.20/schedule.c",
        "isl-0.20/isl_int_imath.h",
        "isl-0.20/include/**/*.h",
    ]) + [
       "isl-0.20/isl_config.h",
       "isl-0.20/isl_srcdir.c",
       "isl-0.20/gitversion.h",
       "isl-0.20/include/isl/stdint.h"],
    includes = [
	"isl-0.20/include",
	#"tools/polly/lib/External/isl/",
	"isl-0.20/",
        "isl-0.20/imath",
    ],
    deps = [
        ":imath",
        ":imath_wrap",
    ],
)
