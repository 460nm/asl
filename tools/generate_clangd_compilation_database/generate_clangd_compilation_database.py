# Copyright 2025 Steven Le Rouzic
#
# SPDX-License-Identifier: BSD-3-Clause

import json
from pathlib import Path
from dataclasses import dataclass
import subprocess
import os
import sys

@dataclass
class PathFragment:
    label: str
    parent_id: int | None = None
    is_final: bool = False

@dataclass
class DepSet:
    direct_artifacts: set[int]
    transitive_dep_sets: set[int]
    is_final: bool = False


def parse_path_fragments(fragments) -> dict[int, PathFragment]:
    def finalize_path_fragment(fragments: dict[int, PathFragment], id: int):
        fragment = fragments[id]
        if fragment.is_final:
            return

        if fragment.parent_id is not None:
            finalize_path_fragment(fragments, fragment.parent_id)
            assert fragments[fragment.parent_id].is_final
            fragment.label = fragments[fragment.parent_id].label + "/" + fragment.label

        fragment.is_final = True

    fragment_per_id = {fragment["id"]: PathFragment(
            label = fragment["label"],
            parent_id = fragment.get("parentId")
        ) for fragment in fragments}

    for id, fragment in fragment_per_id.items():
        finalize_path_fragment(fragment_per_id, id)
        assert fragment.is_final

    return fragment_per_id

def make_artifacts_paths(artifacts, fragments: dict[int, PathFragment]) -> dict[int, str]:
    return {artifact["id"]: fragments[artifact["pathFragmentId"]].label for artifact in artifacts}

def parse_dep_sets(dep_sets) -> dict[int, DepSet]:
    def finalize_dep_set(dep_sets: dict[int, DepSet], id: int):
        dep_set = dep_sets[id]
        if dep_set.is_final:
            return

        for transitive_id in dep_set.transitive_dep_sets:
            finalize_dep_set(dep_sets, transitive_id)
            assert dep_sets[transitive_id].is_final
            dep_set.direct_artifacts.update(dep_sets[transitive_id].direct_artifacts)

        dep_set.is_final = True

    dep_set_per_id = {dep_set["id"]: DepSet(
            direct_artifacts = set(dep_set.get("directArtifactIds", [])),
            transitive_dep_sets = set(dep_set.get("transitiveDepSetIds", []))
        ) for dep_set in dep_sets}

    for id, dep_set in dep_set_per_id.items():
        finalize_dep_set(dep_set_per_id, id)
        assert dep_set.is_final

    return dep_set_per_id

def make_artifacts(data) -> dict[int, str]:
    path_fragments = parse_path_fragments(data["pathFragments"])
    return make_artifacts_paths(data["artifacts"], path_fragments)

if __name__ == "__main__":
    os.chdir(os.getenv("BUILD_WORKSPACE_DIRECTORY"))

    target_args = []
    bazel_args = []

    args = sys.argv[1:]
    if "--" in args:
        sep_index = args.index("--")
        target_args = args[:sep_index]
        bazel_args = args[sep_index+1:]
    else:
        target_args = args


    print("Getting the execution root...")
    ret = subprocess.run([
        "bazel",
        "info",
        "execution_root"],
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
    )
    assert ret.returncode == 0
    exec_root = ret.stdout.decode("utf-8").strip()

    print("Querying Bazel...")

    targets_str = " union ".join(map(lambda t: f"deps({t})", target_args))
    ret = subprocess.run([
        "bazel",
        "aquery",
        f"mnemonic('CppCompile', filter('^//', {targets_str}))",
        "--noshow_progress",
        "--config=windows",
        "--ui_event_filters=-info",
        "--output=jsonproto",
        "--features=-compiler_param_file"],
        stdout=subprocess.PIPE,
    )
    assert ret.returncode == 0

    print("Parsing results...")

    data = json.loads(ret.stdout)
    artifacts = make_artifacts(data)
    dep_sets = parse_dep_sets(data["depSetOfFiles"])
    compile_commands = []

    for action in data["actions"]:
        arguments = action["arguments"][1:]
        candidate_source_files = set()
        for input_id in action.get("inputDepSetIds", []):
            input_dep_set = dep_sets[input_id]
            for artifact_id in input_dep_set.direct_artifacts:
                name = artifacts[artifact_id]
                if name in arguments:
                    candidate_source_files.add(artifact_id)

        assert len(candidate_source_files) == 1

        file = artifacts[candidate_source_files.pop()]
        exec = "clang" if file.endswith(".c") else "clang++"

        compile_commands.append({
            "directory": exec_root,
            "arguments": [exec] + arguments,
            "file": file,
        })

    print(f"Writing {len(compile_commands)} entries to compile_commands.json...")

    compile_commands_content = json.dumps(compile_commands, indent=2)
    Path("compile_commands.json").write_text(compile_commands_content)

    print("Done.")
