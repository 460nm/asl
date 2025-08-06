# Copyright 2025 Steven Le Rouzic
#
# SPDX-License-Identifier: BSD-3-Clause

def _get_system_include_dirs(ctx, clang_path):
    ctx.file("detect.cpp", content = "")
    detect_path = ctx.path("detect.cpp")

    res = ctx.execute([
        clang_path,
        "-v",
        "-no-canonical-prefixes",
        "-fsyntax-only",
        "-x", "c++",
        detect_path,
    ])

    lines = res.stderr.splitlines()
    listing_include_dirs = False
    include_dirs = []

    for l in lines:
        if l.startswith("#include <...> search starts here:"):
            listing_include_dirs = True
        elif l.startswith("End of search list."):
            listing_include_dirs = False
        elif listing_include_dirs:
            include_dirs.append(l.strip())

    return include_dirs

def _host_clang_info_impl(ctx):
    clang_path = ctx.which("clang")
    if not clang_path:
        fail("Could not find clang")

    clangxx_path = ctx.which("clang++")
    if not clang_path:
        fail("Could not find clang++")

    llvmar_path = ctx.which("llvm-ar")
    if not llvmar_path:
        fail("Could not find llvm-ar")

    bash_template = """#!/bin/bash
set -eo pipefail
%s "$@"
"""

    ctx.file("clang.sh", content = bash_template % clang_path, executable = True)
    ctx.file("clangxx.sh", content = bash_template % clangxx_path, executable = True)
    ctx.file("ar.sh", content = bash_template % llvmar_path, executable = True)
    ctx.file("strip.sh", content = "exit 1", executable = True)

    ctx.file("clang.bat", content = "@\"%s\" %%*" % clang_path, executable = True)
    ctx.file("clangxx.bat", content = "@\"%s\" %%*" % clangxx_path, executable = True)
    ctx.file("ar.bat", content = "@\"%s\" %%*" % llvmar_path, executable = True)
    ctx.file("strip.bat", content = "exit 1", executable = True)

    ctx.file("BUILD.bazel", content = """
package(default_visibility = ["//visibility:public"])

filegroup(
    name = "clang",
    srcs = select({
        "@platforms//os:windows": ["clang.bat"],
        "//conditions:default": ["clang.sh"],
    }),
)

filegroup(
    name = "clangxx",
    srcs = select({
        "@platforms//os:windows": ["clangxx.bat"],
        "//conditions:default": ["clangxx.sh"],
    }),
)

filegroup(
    name = "ar",
    srcs = select({
        "@platforms//os:windows": ["ar.bat"],
        "//conditions:default": ["ar.sh"],
    }),
)

filegroup(
    name = "strip",
    srcs = select({
        "@platforms//os:windows": ["strip.bat"],
        "//conditions:default": ["strip.sh"],
    }),
)
""")

    system_include_dirs = _get_system_include_dirs(ctx, clang_path)
    system_include_dirs_str = "\n"
    for p in system_include_dirs:
        system_include_dirs_str += "    \"%s\",\n" % p.replace("\\", "\\\\")

    ctx.file("info.bzl", content = """
SYSTEM_INCLUDE_DIRS = [%s]\n
""" % system_include_dirs_str)

_host_clang_info = repository_rule(
    implementation = _host_clang_info_impl,
    local = True,
    configure = True,
)

host_clang_info = module_extension(
    implementation = lambda ctx: _host_clang_info(name = "clang_info"),
)
