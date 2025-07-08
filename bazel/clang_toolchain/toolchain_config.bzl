# Copyright 2025 Steven Le Rouzic
#
# SPDX-License-Identifier: BSD-3-Clause

load(
    "//:clang_config.bzl",
    "CLANG_BINDIR",
    "CLANG_LIBDIR",
    "CLANG_INCLUDE_DIRS",
)

load("@rules_cc//cc:action_names.bzl", "ACTION_NAMES")

load(
    "@rules_cc//cc:cc_toolchain_config_lib.bzl",
    "action_config",
    "artifact_name_pattern",
    "feature",
    "feature_set",
    "flag_group",
    "flag_set",
    "tool",
    "tool_path",
    "variable_with_value",
)

CPP_COMPILE_ACTIONS = [
    ACTION_NAMES.cpp_compile,
    ACTION_NAMES.cpp_header_parsing
]

C_COMPILE_ACTIONS = [
    ACTION_NAMES.c_compile,
]

COMPILE_ACTIONS = C_COMPILE_ACTIONS + CPP_COMPILE_ACTIONS

LINK_ACTIONS = [
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
    ACTION_NAMES.cpp_link_executable,
]

ARCHIVE_ACTIONS = [
    ACTION_NAMES.cpp_link_static_library,
]

def _impl(ctx):
    os = ctx.attr.os
    is_windows = os == "windows"
    exe_suffix = ".exe" if is_windows else ""

    tool_paths = [
        tool_path(name = "ar",      path = CLANG_BINDIR + "/llvm-ar" + exe_suffix),
        tool_path(name = "ld",      path = CLANG_BINDIR + "/ld.lld" + exe_suffix),
        tool_path(name = "nm",      path = CLANG_BINDIR + "/llvm-nm" + exe_suffix),
        tool_path(name = "objdump", path = CLANG_BINDIR + "/llvm-objdump" + exe_suffix),
        tool_path(name = "strip",   path = CLANG_BINDIR + "/llvm-strip" + exe_suffix),
        tool_path(name = "gcc",     path = CLANG_BINDIR + "/clang" + exe_suffix),
        tool_path(name = "cpp",     path = CLANG_BINDIR + "/clang++" + exe_suffix),
    ]

    action_configs = [
        action_config(action_name = name, enabled = True, tools = [tool(path = CLANG_BINDIR + "/clang" + exe_suffix)])
        for name in C_COMPILE_ACTIONS
    ] + [
        action_config(action_name = name, enabled = True, tools = [tool(path = CLANG_BINDIR + "/clang++" + exe_suffix)])
        for name in CPP_COMPILE_ACTIONS
    ] + [
        action_config(action_name = name, enabled = True, tools = [tool(path = CLANG_BINDIR + "/clang++" + exe_suffix)])
        for name in LINK_ACTIONS
    ] + [
        action_config(action_name = name, enabled = True, tools = [tool(path = CLANG_BINDIR + "/llvm-ar" + exe_suffix)])
        for name in ARCHIVE_ACTIONS
    ] + [
        action_config(ACTION_NAMES.strip, enabled = True, tools = [tool(path = CLANG_BINDIR + "/llvm-strip" + exe_suffix)]),
    ]

    artifact_name_patterns = [
        artifact_name_pattern(
            category_name = "object_file",
            prefix = "",
            extension = ".obj" if is_windows else ".o",
        ),
        artifact_name_pattern(
            category_name = "static_library",
            prefix = "" if is_windows else "lib",
            extension = ".lib" if is_windows else ".a",
        ),
        artifact_name_pattern(
            category_name = "dynamic_library",
            prefix = "" if is_windows else "lib",
            extension = ".dll" if is_windows else ".so",
        ),
        artifact_name_pattern(
            category_name = "executable",
            prefix = "",
            extension = exe_suffix,
        ),
    ]

    asan_link_flag_sets = []
    ubsan_link_flag_sets = []

    if is_windows:
        asan_link_flag_sets.append(flag_set(
            actions = LINK_ACTIONS,
            flag_groups = [
                flag_group(
                    flags = [
                        "-l\"" + CLANG_LIBDIR + "/lib/windows/clang_rt.asan_dynamic-x86_64.lib\"",
                    ],
                ),
            ],
        ))

        ubsan_link_flag_sets.append(flag_set(
            actions = LINK_ACTIONS,
            flag_groups = [
                flag_group(
                    flags = [
                        "-l\"" + CLANG_LIBDIR + "/lib/windows/clang_rt.ubsan_standalone-x86_64.lib\"",
                    ],
                ),
            ],
        ))

    features = [
        feature(name = "no_legacy_features"),
        feature(name = "opt"),
        feature(name = "fastbuild"),
        feature(name = "dbg"),
        feature(
            name = "default_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = COMPILE_ACTIONS + LINK_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-no-canonical-prefixes",
                                "-fcolor-diagnostics",
                            ],
                        ),
                    ],
                ),
                flag_set(
                    actions = LINK_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-fuse-ld=lld",
                            ],
                        ),
                    ],
                ),
                flag_set(
                    actions = COMPILE_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-Wno-builtin-macro-redefined",
                                "-D__DATE__=\"redacted\"",
                                "-D__TIMESTAMP__=\"redacted\"",
                                "-D__TIME__=\"redacted\"",
                                "-c",
                            ],
                        ),
                        flag_group(
                            flags = ["-frandom-seed=%{output_file}"],
                            expand_if_available = "output_file",
                        ),
                        flag_group(
                            flags = ["-D%{preprocessor_defines}"],
                            iterate_over = "preprocessor_defines",
                        ),
                        flag_group(
                            flags = ["-I%{include_paths}"],
                            iterate_over = "include_paths",
                        ),
                        flag_group(
                            flags = ["-iquote", "%{quote_include_paths}"],
                            iterate_over = "quote_include_paths",
                        ),
                        flag_group(
                            flags = ["-isystem", "%{system_include_paths}"],
                            iterate_over = "system_include_paths",
                        ),
                        flag_group(
                            flags = ["-MD", "-MF", "%{dependency_file}"],
                            expand_if_available = "dependency_file",
                        ),
                    ],
                ),
                flag_set(
                    actions = ARCHIVE_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = ["rcsD"],
                        ),
                        flag_group(
                            flags = ["%{output_execpath}"],
                            expand_if_available = "output_execpath",
                        ),
                    ],
                ),
                flag_set(
                    actions = LINK_ACTIONS + ARCHIVE_ACTIONS,
                    flag_groups = [
                        flag_group(
                            iterate_over = "libraries_to_link",
                            expand_if_available = "libraries_to_link",
                            flag_groups = [
                                flag_group(
                                    flags = ["-Wl,--start-lib"],
                                    expand_if_equal = variable_with_value(
                                        name = "libraries_to_link.type",
                                        value = "object_file_group",
                                    ),
                                ),
                                flag_group(
                                    iterate_over = "libraries_to_link.object_files",
                                    expand_if_equal = variable_with_value(
                                        name = "libraries_to_link.type",
                                        value = "object_file_group",
                                    ),
                                    flag_groups = [
                                        flag_group(
                                            flags = ["%{libraries_to_link.object_files}"],
                                            expand_if_false = "libraries_to_link.is_whole_archive",
                                        ),
                                        flag_group(
                                            flags = ["-Wl,-force_load,%{libraries_to_link.object_files}"],
                                            expand_if_true = "libraries_to_link.is_whole_archive",
                                        ),
                                    ],
                                ),
                                flag_group(
                                    expand_if_equal = variable_with_value(
                                        name = "libraries_to_link.type",
                                        value = "object_file",
                                    ),
                                    flag_groups = [
                                        flag_group(
                                            flags = ["%{libraries_to_link.name}"],
                                            expand_if_false = "libraries_to_link.is_whole_archive",
                                        ),
                                        flag_group(
                                            flags = ["-Wl,-force_load,%{libraries_to_link.name}"],
                                            expand_if_true = "libraries_to_link.is_whole_archive",
                                        ),
                                    ],
                                ),
                                flag_group(
                                    expand_if_equal = variable_with_value(
                                        name = "libraries_to_link.type",
                                        value = "interface_library",
                                    ),
                                    flag_groups = [
                                        flag_group(
                                            flags = ["%{libraries_to_link.name}"],
                                            expand_if_false = "libraries_to_link.is_whole_archive",
                                        ),
                                        flag_group(
                                            flags = ["-Wl,-force_load,%{libraries_to_link.name}"],
                                            expand_if_true = "libraries_to_link.is_whole_archive",
                                        ),
                                    ],
                                ),
                                flag_group(
                                    expand_if_equal = variable_with_value(
                                        name = "libraries_to_link.type",
                                        value = "static_library",
                                    ),
                                    flag_groups = [
                                        flag_group(
                                            flags = ["%{libraries_to_link.name}"],
                                            expand_if_false = "libraries_to_link.is_whole_archive",
                                        ),
                                        flag_group(
                                            flags = ["-Wl,-force_load,%{libraries_to_link.name}"],
                                            expand_if_true = "libraries_to_link.is_whole_archive",
                                        ),
                                    ],
                                ),
                                flag_group(
                                    flags = ["-l%{libraries_to_link.name}"],
                                    expand_if_equal = variable_with_value(
                                        name = "libraries_to_link.type",
                                        value = "dynamic_library",
                                    ),
                                ),
                                flag_group(
                                    flags = ["-l:%{libraries_to_link.name}"],
                                    expand_if_equal = variable_with_value(
                                        name = "libraries_to_link.type",
                                        value = "versioned_dynamic_library",
                                    ),
                                ),
                                flag_group(
                                    expand_if_true = "libraries_to_link.is_whole_archive",
                                    flag_groups = [
                                        flag_group(
                                            expand_if_false = "macos_flags",
                                            flags = ["-Wl,-no-whole-archive"],
                                        ),
                                    ],
                                ),
                                flag_group(
                                    flags = ["-Wl,--end-lib"],
                                    expand_if_equal = variable_with_value(
                                        name = "libraries_to_link.type",
                                        value = "object_file_group",
                                    ),
                                ),
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "windows_linker_flags",
            enabled = is_windows,
            flag_sets = [
                flag_set(
                    actions = LINK_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-Xlinker",
                                "/subsystem:console",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "dbg_flags",
            enabled = True,
            requires = [feature_set(["dbg"])],
            flag_sets = [
                flag_set(
                    actions = COMPILE_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-DDEBUG=1",
                            ],
                        ),
                    ],
                ),
                flag_set(
                    actions = COMPILE_ACTIONS + LINK_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-g3",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "fastbuild_flags",
            enabled = True,
            requires = [feature_set(["fastbuild"])],
            flag_sets = [
                flag_set(
                    actions = COMPILE_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-O1",
                                "-DDEBUG=1",
                            ],
                        ),
                    ],
                ),
                flag_set(
                    actions = COMPILE_ACTIONS + LINK_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-g1",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "opt_flags",
            enabled = True,
            requires = [feature_set(["opt"])],
            flag_sets = [
                flag_set(
                    actions = COMPILE_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-O3",
                                "-DNDEBUG=1",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "c++20",
            provides = ["c++_version"],
            flag_sets = [
                flag_set(
                    actions = COMPILE_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = ["-std=c++20"],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "c++23",
            provides = ["c++_version"],
            flag_sets = [
                flag_set(
                    actions = COMPILE_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = ["-std=c++23"],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "asan",
            implies = ["sanitize"],
            flag_sets = [
                flag_set(
                    actions = COMPILE_ACTIONS + LINK_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-fsanitize=address",
                            ],
                        ),
                    ],
                ),
            ] + asan_link_flag_sets,
        ),
        feature(
            name = "ubsan",
            implies = ["sanitize"],
            flag_sets = [
                flag_set(
                    actions = COMPILE_ACTIONS + LINK_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-fsanitize=undefined",
                            ],
                        ),
                    ],
                ),
            ] + ubsan_link_flag_sets,
        ),
        feature(
            name = "sanitize",
            flag_sets = [
                flag_set(
                    actions = COMPILE_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-fno-omit-frame-pointer",
                                "-fno-sanitize-ignorelist",
                                "-fno-sanitize-recover=all",
                            ],
                        ),
                    ],
                ),
            ],
        ),
        feature(
            name = "final_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = COMPILE_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = ["%{user_compile_flags}"],
                            expand_if_available = "user_compile_flags",
                            iterate_over = "user_compile_flags",
                        ),
                        flag_group(
                            flags = ["-o", "%{output_file}"],
                            expand_if_available = "output_file",
                        ),
                        flag_group(
                            flags = ["%{source_file}"],
                            expand_if_available = "source_file",
                        ),
                    ],
                ),
                flag_set(
                    actions = LINK_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = ["%{user_link_flags}"],
                            expand_if_available = "user_link_flags",
                            iterate_over = "user_link_flags",
                        ),
                        flag_group(
                            flags = ["-o", "%{output_execpath}"],
                            expand_if_available = "output_execpath",
                        ),
                    ],
                ),
                flag_set(
                    actions = LINK_ACTIONS + ARCHIVE_ACTIONS,
                    flag_groups = [
                        flag_group(
                            flags = ["@%{linker_param_file}"],
                            expand_if_available = "linker_param_file",
                        ),
                    ],
                ),
            ],
        ),
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        toolchain_identifier = "%s-toolchain" % os,
        compiler = "clang",
        artifact_name_patterns = artifact_name_patterns,
        cxx_builtin_include_directories = CLANG_INCLUDE_DIRS,
        tool_paths = tool_paths,
        action_configs = action_configs,
        features = features,
    )

toolchain_config = rule(
    implementation = _impl,
    attrs = {
        "os": attr.string(mandatory = True),
    },
    provides = [CcToolchainConfigInfo],
)

def _declare_toolchain_impl(name, os, arch, visibility):
    toolchain_config(
        name = name + "_config",
        os = os,
    )

    native.cc_toolchain(
        name = name + "_cc",
        toolchain_config = name + "_config",
        all_files = ":empty",
        compiler_files = ":empty",
        linker_files = ":empty",
        objcopy_files = ":empty",
        strip_files = ":empty",
        dwp_files = ":empty",
    )

    native.toolchain(
        name = name + "_def",
        toolchain = name + "_cc",
        toolchain_type = "@rules_cc//cc:toolchain_type",
        exec_compatible_with = [
            "@platforms//cpu:%s" % arch,
            "@platforms//os:%s" % os,
        ],
        target_compatible_with = [
            "@platforms//cpu:%s" % arch,
            "@platforms//os:%s" % os,
        ],
        visibility = ["//visibility:public"],
    )

declare_toolchain = macro(
    implementation = _declare_toolchain_impl,
    attrs = {
        "os":   attr.string(mandatory = True, configurable = False),
        "arch": attr.string(mandatory = True, configurable = False),
    }
)
