#!/usr/bin/env python3

import argparse
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


def preprocess_includes(include_lines: list[str]) -> list[str]:
    if not include_lines:
        return []

    with tempfile.TemporaryDirectory() as temp_dir:
        temp_path = pathlib.Path(temp_dir) / "submit_includes.cpp"
        temp_path.write_text("".join(include_lines))

        cmd = ["g++", "-std=c++20"]
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


def generate_submit(problem_dir: pathlib.Path) -> None:
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
    expanded_includes = preprocess_includes(include_lines)

    output_lines = collapse_blank_lines(template_prefix + expanded_includes + body_lines)
    output_path.write_text("".join(output_lines))


def configure() -> None:
    BUILD_DIR.mkdir(exist_ok=True)
    subprocess.run(
        ["cmake", "..", "-G", "Ninja"],
        cwd=BUILD_DIR,
        check=True,
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
    elif info.startswith("macos-13"):
        candidates.extend(
            [
                "/usr/local/bin/time",
                "/opt/homebrew/bin/gtime",
            ]
        )
    elif info.startswith("macos-15"):
        candidates.extend(
            [
                "/Users/xdbwk/local/time-1.9/time",
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

    problem_dir = ensure_problem(args.name)
    generate_submit(problem_dir)
    configure()
    build(problem_dir.name)
    run_binary(problem_dir)


if __name__ == "__main__":
    main()
