# Copyright 2025 Steven Le Rouzic
#
# SPDX-License-Identifier: BSD-3-Clause

def _copy_file(ctx, from_path, to_path):
    content = ctx.read(from_path)
    ctx.file(to_path, content = content)

def _get_clang_info(ctx, clang_path, line_prefix):
    result = ctx.execute([clang_path, "--version"])
    if result.return_code != 0:
        return None

    for l in result.stdout.splitlines():
        if l.startswith(line_prefix):
            l = l[len(line_prefix):]
            return l.split(" ")[0]

    return None

def _get_clang_version(ctx, clang_path):
    return _get_clang_info(ctx, clang_path, "clang version ")

def _get_clang_lib_dir(ctx, clang_path):
    result = ctx.execute([clang_path, "-no-canonical-prefixes", "--print-resource-dir"])
    if result.return_code != 0:
        return None

    return result.stdout

def _get_include_dirs(ctx, clang_path):
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

def _impl(ctx):
    _copy_file(ctx, ctx.attr._build_bazel, "BUILD.bazel")
    _copy_file(ctx, ctx.attr._toolchain_config_bzl, "toolchain_config.bzl")

    clang_path = ctx.which("clang")
    if not clang_path:
        fail("Could not find clang")

    clang_version = _get_clang_version(ctx, clang_path)
    if not clang_version:
        fail("Could not detect clang version")

    clang_lib_dir = _get_clang_lib_dir(ctx, clang_path)
    if not clang_lib_dir:
        fail("Could not detect clang lib dir")

    include_dirs = _get_include_dirs(ctx, clang_path)
    if not include_dirs:
        fail("Could not detect include dirs")

    ctx.template(
        "clang_config.bzl",
        ctx.attr._clang_config_bzl_tpl,
        executable = False,
        substitutions = {
            "{CLANG_BINDIR}": str(clang_path.dirname),
            "{CLANG_LIBDIR}": str(clang_lib_dir).strip().replace("\\", "/"),
            "{CLANG_INCLUDE_DIRS}": str(include_dirs),
        },
    )


_config_detection = repository_rule(
    implementation = _impl,
    local = True,
    configure = True,
    attrs = {
        "_build_bazel": attr.label(
            default = Label("//bazel/clang_toolchain:clang_toolchain.BUILD.bazel"),
            allow_single_file = True,
        ),
        "_toolchain_config_bzl": attr.label(
            default = Label("//bazel/clang_toolchain:toolchain_config.bzl"),
            allow_single_file = True,
        ),
        "_clang_config_bzl_tpl": attr.label(
            default = Label("//bazel/clang_toolchain:clang_config.bzl.tpl"),
            allow_single_file = True,
        ),
    },
)

clang_toolchain = module_extension(
    implementation = lambda ctx: _config_detection(
        name = "clang_toolchain",
    ),
)
