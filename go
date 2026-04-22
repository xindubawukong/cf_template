#!/usr/bin/env python3

import argparse
import os
import pathlib
import platform
import re
import shutil
import subprocess
import tempfile


ROOT = pathlib.Path(__file__).resolve().parent
BUILD_DIR = ROOT / "build"
TEMPLATE_MAIN = ROOT / "templates" / "main.cpp"
INCLUDE_DIRS = [
    ROOT,
    ROOT / "templates",
    ROOT / "utils",
    ROOT / "utils/system_headers",
    ROOT / "third_party" / "parlaylib" / "include",
]


def ensure_problem(name: str) -> pathlib.Path:
    problem_dir = ROOT / f"problem_{name}"
    problem_dir.mkdir(exist_ok=True)

    problem_main = problem_dir / "main.cpp"
    if not problem_main.exists():
        shutil.copyfile(TEMPLATE_MAIN, problem_main)

    problem_input = problem_dir / "input.txt"
    problem_input.touch(exist_ok=True)

    return problem_dir


def find_compilers() -> tuple[str, str]:
    clang = shutil.which("clang")
    clangxx = shutil.which("clang++")
    if clang and clangxx:
        return clang, clangxx

    gcc = shutil.which("gcc")
    gxx = shutil.which("g++")
    if gcc and gxx:
        return gcc, gxx

    raise FileNotFoundError(
        "Neither clang/clang++ nor gcc/g++ is available"
    )


def read_cached_compiler(cache_path: pathlib.Path, key: str) -> str | None:
    if not cache_path.exists():
        return None

    pattern = re.compile(rf"^{re.escape(key)}(?::[^=]+)?=(.*)$")
    for line in cache_path.read_text().splitlines():
        match = pattern.match(line)
        if match:
            return match.group(1)
    return None


def same_compiler(first: str | None, second: str) -> bool:
    if not first:
        return False
    return pathlib.Path(first).resolve() == pathlib.Path(second).resolve()


def reset_cmake_cache_if_needed(cc_compiler: str, cxx_compiler: str) -> None:
    cache_path = BUILD_DIR / "CMakeCache.txt"
    cached_cc = read_cached_compiler(cache_path, "CMAKE_C_COMPILER")
    cached_cxx = read_cached_compiler(cache_path, "CMAKE_CXX_COMPILER")

    if same_compiler(cached_cc, cc_compiler) and same_compiler(cached_cxx, cxx_compiler):
        return

    if cache_path.exists():
        cache_path.unlink()

    cmake_files_dir = BUILD_DIR / "CMakeFiles"
    if cmake_files_dir.exists():
        shutil.rmtree(cmake_files_dir)


def remove_path(path: pathlib.Path) -> None:
    if path.is_dir():
        shutil.rmtree(path)
    else:
        path.unlink()


def clean() -> None:
    targets = []
    targets.extend(
        sorted(path for path in ROOT.glob("problem_*") if path.is_dir())
    )
    if BUILD_DIR.exists():
        targets.extend(sorted(BUILD_DIR.glob("problem_*")))

        cmake_files_dir = BUILD_DIR / "CMakeFiles"
        if cmake_files_dir.exists():
            targets.extend(sorted(cmake_files_dir.glob("problem_*")))

    if not targets:
        print("Nothing to clean.")
        return

    try:
        answer = input(
            "Clean all problem_* directories and corresponding build files? [y/N] "
        )
    except EOFError:
        answer = ""

    if answer.strip().lower() not in {"y", "yes"}:
        print("Aborted.")
        return

    for path in targets:
        if path.exists():
            remove_path(path)

    print(f"Removed {len(targets)} path(s).")


def read_lines(path: pathlib.Path) -> list[str]:
    return path.read_text().splitlines(keepends=True)


def find_namespace_line(lines: list[str], path: pathlib.Path) -> int:
    for idx, line in enumerate(lines):
        if "using namespace std;" in line:
            return idx
    raise ValueError(f"missing 'using namespace std;' in {path}")


def collapse_blank_lines(lines: list[str]) -> list[str]:
    result = []
    prev_blank = False
    for line in lines:
        blank = line.strip() == ""
        if blank and prev_blank:
            continue
        result.append(line)
        prev_blank = blank
    return result


def preprocess_includes(include_lines: list[str], cxx_compiler: str) -> list[str]:
    if not include_lines:
        return []

    with tempfile.TemporaryDirectory() as temp_dir:
        temp_path = pathlib.Path(temp_dir) / "submit_includes.cpp"
        temp_path.write_text("".join(include_lines))

        cmd = [cxx_compiler, "-std=c++20"]
        for include_dir in INCLUDE_DIRS:
            cmd.extend(["-I", str(include_dir)])
        cmd.extend(["-E", str(temp_path)])

        result = subprocess.run(
            cmd,
            cwd=ROOT,
            check=True,
            capture_output=True,
            text=True,
        )

    cleaned = []
    for line in result.stdout.splitlines(keepends=True):
        if re.match(r"# \d+.*", line):
            cleaned.append("\n")
        else:
            cleaned.append(line)
    return collapse_blank_lines(cleaned)


def generate_submit(problem_dir: pathlib.Path, cxx_compiler: str) -> None:
    source_path = problem_dir / "main.cpp"
    output_path = problem_dir / "submit.cpp"

    template_lines = read_lines(TEMPLATE_MAIN)
    source_lines = read_lines(source_path)

    template_split = find_namespace_line(template_lines, TEMPLATE_MAIN)
    source_split = find_namespace_line(source_lines, source_path)

    template_prefix = template_lines[: template_split + 1]
    source_body = source_lines[source_split + 1 :]

    include_lines = [line for line in source_body if line.startswith("#include")]
    body_lines = [line for line in source_body if not line.startswith("#include")]
    expanded_includes = preprocess_includes(include_lines, cxx_compiler)

    output_lines = collapse_blank_lines(template_prefix + expanded_includes + body_lines)
    output_path.write_text("".join(output_lines))


def configure(cc_compiler: str, cxx_compiler: str) -> None:
    BUILD_DIR.mkdir(exist_ok=True)
    reset_cmake_cache_if_needed(cc_compiler, cxx_compiler)
    env = os.environ.copy()
    env["CC"] = cc_compiler
    env["CXX"] = cxx_compiler
    subprocess.run(
        ["cmake", "..", "-G", "Ninja"],
        cwd=BUILD_DIR,
        check=True,
        env=env,
    )


def build(problem_target: str) -> None:
    subprocess.run(
        ["ninja", problem_target],
        cwd=BUILD_DIR,
        check=True,
    )


def get_gnu_time() -> str:
    info = platform.platform().lower()
    candidates = []
    if info.startswith("linux"):
        candidates.extend(
            [
                "/home/xdbwk/local/time-1.9/time",
                "/usr/bin/time",
            ]
        )
    elif info.startswith("macos"):
        candidates.extend(
            [
                "/usr/local/bin/time",
                "/opt/homebrew/bin/gtime",
            ]
        )

    for candidate in candidates:
        if pathlib.Path(candidate).exists():
            return candidate

    raise FileNotFoundError("GNU time is not installed")


def print_time_stats(problem_name: str, stats_path: pathlib.Path) -> None:
    lines = stats_path.read_text().splitlines()
    time_value = 0.0
    memory_value = 0.0
    bad = ""
    for line in lines:
        if "User time" in line:
            time_value = float(line.split()[-1])
        if "Maximum resident set size" in line:
            memory_value = float(line.split()[-1]) / 1000
        if "Command terminated by signal" in line:
            bad = line
    if bad:
        for line in lines:
            if not line.startswith("\t"):
                print(line)
    print(f"[{problem_name}] time = {time_value} s, memory = {memory_value} MB")


def run_binary(problem_dir: pathlib.Path) -> None:
    binary = BUILD_DIR / problem_dir.name / "main"
    input_file = problem_dir / "input.txt"
    stats_file = BUILD_DIR / f"{problem_dir.name}_run.txt"
    time_bin = get_gnu_time()
    with input_file.open("r") as stdin:
        with stats_file.open("w") as stderr:
            result = subprocess.run(
                [time_bin, "-v", str(binary)],
                stdin=stdin,
                stderr=stderr,
                check=False,
            )
    print_time_stats(problem_dir.name, stats_file)
    if result.returncode != 0:
        raise subprocess.CalledProcessError(result.returncode, [time_bin, "-v", str(binary)])


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("name")
    args = parser.parse_args()
    cc_compiler, cxx_compiler = find_compilers()

    if args.name == "clean":
        clean()
        return

    problem_dir = ensure_problem(args.name)
    generate_submit(problem_dir, cxx_compiler)
    configure(cc_compiler, cxx_compiler)
    build(problem_dir.name)
    run_binary(problem_dir)


if __name__ == "__main__":
    main()
