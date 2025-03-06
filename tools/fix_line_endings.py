# Copyright 2025 Steven Le Rouzic
#
# SPDX-License-Identifier: BSD-3-Clause

import os
import subprocess
import glob
import re
import sys
import time

TO_FIX = [
    ".bazelrc",
    ".clang-tidy",
    ".clangd",
    ".gitignore",
    "**/*.hpp",
    "**/*.h",
    "**/*.cpp",
    "**/*.py",
    "**/*.bzl",
    "**/*.bazel",
    "**/*.txt",
    "**/*.bat",
    "**/*.sh",
]

TO_IGNORE = [
    "MODULE.bazel.lock",
]

TO_FIX = [re.compile(glob.translate(p, recursive=True, include_hidden=True)) for p in TO_FIX]
TO_IGNORE = [re.compile(glob.translate(p, recursive=True, include_hidden=True)) for p in TO_IGNORE]

def get_git_files():
    return subprocess.check_output(["git", "ls-files"]).decode().splitlines()

def fix_file(file):
    lines = []
    with open(file, "rb") as fp:
        lines = [l.rstrip() + b"\n" for l in fp.readlines()]
    with open(file, "wb+") as fp:
        fp.writelines(lines)

if __name__ == "__main__":
    os.chdir(os.getenv("BUILD_WORKSPACE_DIRECTORY"))

    files = get_git_files()
    files_to_fix = []
    unhandled_files = []

    for f in files:
        for pattern in TO_FIX:
            if pattern.match(f):
                files_to_fix.append(f)
                break
        else:
            for pattern in TO_IGNORE:
                if pattern.match(f):
                    break
            else:
                unhandled_files.append(f)

    if len(unhandled_files):
        print("Some files were not handled:")
        for f in unhandled_files:
            print("    ", f)
        sys.exit(1)

    for i, f in enumerate(files):
        print(f"\033[K{i}/{len(files_to_fix)} {f}", end="\r", flush=True)
        fix_file(f)
    print("")
