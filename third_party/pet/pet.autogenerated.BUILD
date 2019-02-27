# Bazel BUILD file for ISL.
#

licenses(["notice"])

exports_files(["LICENSE.TXT"])

load(
    "@org_tensorflow//third_party/pet:pet.bzl",
    "expand_isl_cmake_vars",
    "isl_all_cmake_vars",
)

package(default_visibility = ["//visibility:public"])



# Genrule to edit header file which avoids multiple definitions error
# TODO: Ask ISL why have they removed this definition from version 0.19 onwards
genrule(
    name = "correct_imath_wrap_h",
    srcs = ["pet/isl/imath_wrap/wrap.h"],
    outs = [],
    cmd = "sed 's/#endif/#define impz_fdiv_q_ui\tisl_impz_fdiv_q_ui\n#endif' $@",
)


# Isl - Config file.
expand_isl_cmake_vars(
    name = "isl_config",
    src = "pet/isl/isl_config.h.in",
    cmake_vars = isl_all_cmake_vars,
    dst = "pet/isl/isl_config.h",
)

expand_isl_cmake_vars(
    name = "isl_srcdir",
    src = "pet/isl/isl_srcdir.c.in",
    cmake_vars = isl_all_cmake_vars,
    dst = "pet/isl/isl_srcdir.c",
)

genrule(
    name = "configure_stdint_h",
    outs = ["pet/isl/include/isl/stdint.h"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "#include <stdint.h>",
        "",
        "EOF",
    ]),
)


genrule(
    name = "gen_gitversion_h",
    srcs = ["pet/isl/GIT_HEAD_ID"],
    outs = ["pet/isl/gitversion.h"],
    cmd = "echo '#define GIT_HEAD_ID \"isl-0.20-1047-g4a20ef8\"' | tee $@",
)



cc_library(
    name = "isl_test_inputs",
    srcs = glob([
        "pet/isl/test_inputs/*.c",
        "pet/isl/test_inputs/*.cpp",
    ]),
    hdrs = glob([
        "pet/isl/test_inputs/*.h",
    ]),
)

cc_library(
    name = "imath",
    srcs = glob([
        "pet/isl/imath/*.c",
        "pet/isl/imath/*.cpp",
    ]),
    hdrs = glob([
        "pet/isl/imath/*.h",
    ]),
)

cc_library(
    name = "imath_wrap",
    srcs = glob([
        "pet/isl/imath_wrap/*.c",
        "pet/isl/imath_wrap/*.cpp",
    ]),
    hdrs = glob([
        "pet/isl/imath_wrap/*.h",
        "pet/isl/imath/*.h",
        "pet/isl/imath/*.c",
    ]),
)

cc_library(
    name = "pet_isl",
    srcs = glob([
	    "pet/isl/*.c",
	    "pet/isl/*.cpp",
        ], exclude = [
	    "pet/isl/*templ.c",
	    "pet/isl/isl_pw_hash.c",
	    "pet/isl/isl_multi_apply_union_set.c",
	    "pet/isl/isl_multi_gist.c",
	    "pet/isl/isl_pw_union_opt.c",
	    "pet/isl/isl_multi_cmp.c",
	    "pet/isl/isl_multi_templ.c",
	    "pet/isl/isl_multi_dims.c",
	    "pet/isl/isl_multi_align_set.c",
	    "pet/isl/isl_multi_align_union_set.c",
            "pet/isl/isl_multi_union_pw_aff_explicit_domain.c",
	    "pet/isl/isl_multi_no_explicit_domain.c",
	    "pet/isl/extract_key.c",
	    "pet/isl/isl_pw_eval.c",
	    "pet/isl/schedule_cmp.c",
 	    "pet/isl/flow_cmp.c",
	    "pet/isl/isl_multi_explicit_domain.c",
	    "pet/isl/isl_multi_pw_aff_explicit_domain.c",
	    "pet/isl/print_templ.c",
	    "pet/isl/isl_union_neg.c",
	    "pet/isl/isl_multi_coalesce.c",
	    "pet/isl/isl_multi_apply_set.c",
	    #"pet/isl/isl_int_sioimath.c",
	    "pet/isl/isl_multi_intersect.c",
	    "pet/isl/isl_val_sioimath.c",
	    "pet/isl/print_templ_yaml.c",
	    "pet/isl/isl_ast_int.c",
	    "pet/isl/isl_val_gmp.c",
	    "pet/isl/isl_union_single.c",
	    "pet/isl/isl_multi_hash.c",
	    "pet/isl/mp_get_memory_functions.c",
	    "pet/isl/isl_union_eval.c",
	    "pet/isl/isl_multi_floor.c",
	    "pet/isl/isl_gmp.c",
	    "pet/isl/isl_union_multi.c",
	    "pet/isl/isl_test_int.c",
	    "pet/isl/isl_test_imath.c",
	    "pet/isl/bound.c",
	    "pet/isl/cat.c",
	    "pet/isl/closure.c",
	    "pet/isl/codegen.c",
	    "pet/isl/isl_test.c",
	    "pet/isl/isl_test_imath.c",
            "pet/isl/pip.c",
	    "pet/isl/polyhedron_detect_equalities.c",
	    "pet/isl/polyhedron_minimize.c",
	    "pet/isl/polyhedron_sample.c",
	    "pet/isl/polytope_scan.c",
	    "pet/isl/schedule.c",
	]),
    hdrs = glob([
        "pet/isl/isl_pw_hash.c",
        "pet/isl/isl_multi_gist.c",
        "pet/isl/isl_pw_union_opt.c",
        "pet/isl/read_in_string_templ.c",
        "pet/isl/isl_multi_cmp.c",
        "pet/isl/isl_multi_templ.c",
        "pet/isl/isl_tab_lexopt_templ.c",
        "pet/isl/print_templ.c",
        "pet/isl/isl_multi_apply_templ.c",
        "pet/isl/isl_multi_align_union_set.c",
        "pet/isl/isl_union_neg.c",
        "pet/isl/*.h",
        "pet/isl/*templ.c",
        "pet/isl/isl_multi_union_pw_aff_explicit_domain.c",
        "pet/isl/isl_multi_no_explicit_domain.c",
        "pet/isl/extract_key.c",
        "pet/isl/isl_pw_eval.c",
        "pet/isl/isl_multi_explicit_domain.c",
        "pet/isl/isl_multi_pw_aff_explicit_domain.c",
	"pet/isl/isl_multi_coalesce.c",
	"pet/isl/isl_multi_apply_set.c",
	"pet/isl/isl_int_sioimath.c",
	"pet/isl/isl_multi_intersect.c",
	"pet/isl/isl_val_sioimath.c",
        #"pet/isl/schedule_cmp.c",
        #"pet/isl/flow_cmp.c",
	"pet/isl/print_templ_yaml.c",
	"pet/isl/isl_val_gmp.c",
	"pet/isl/isl_union_single.c",
	"pet/isl/isl_multi_hash.c",
	"pet/isl/mp_get_memory_functions.c",
	"pet/isl/isl_union_eval.c",
	"pet/isl/isl_multi_floor.c",
	"pet/isl/isl_union_multi.c",
	"pet/isl/isl_test_int.c",
	"pet/isl/isl_test_imath.c",
	"pet/isl/bound.c",
	"pet/isl/cat.c",
	"pet/isl/closure.c",
	"pet/isl/codegen.c",
	"pet/isl/isl_test.c",
	"pet/isl/isl_test_imath.c",
	"pet/isl/isl_test_int.c",
	"pet/isl/pip.c",
	"pet/isl/polyhedron_detect_equalities.c",
	"pet/isl/polyhedron_minimize.c",
	"pet/isl/polyhedron_sample.c",
	"pet/isl/polytope_scan.c",
	"pet/isl/schedule.c",
        "pet/isl/isl_int_imath.h",
        "pet/isl/include/**/*.h",
    ]) + [
       "pet/isl/isl_config.h",
       "pet/isl/isl_srcdir.c",
       "pet/isl/gitversion.h",
       "pet/isl/include/isl/stdint.h"],
    includes = [
	"pet/isl/include",
	#"tools/polly/lib/External/isl/",
	"pet/isl/",
        "pet/isl/imath",
    ],
    deps = [
        ":imath",
        ":imath_wrap",
    ],
)

cc_library(
    name="pet",
    srcs = glob ([
    ]),
    hdrs = [
	"pet/include/pet.h",
    ],
    includes = [
    "pet/isl/include"
    ],
    deps = [
        ":pet_isl",
    ],
)

