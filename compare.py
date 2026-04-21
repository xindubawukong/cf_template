#!/usr/bin/env python3

import argparse
import filecmp
import pathlib
import subprocess


ROOT = pathlib.Path(__file__).resolve().parent
BUILD_DIR = ROOT / "build"


def problem_dir(name: str) -> pathlib.Path:
    return ROOT / f"problem_{name}"


def problem_binary(name: str) -> pathlib.Path:
    return BUILD_DIR / f"problem_{name}" / "main"


def validate_problem(name: str) -> None:
    directory = problem_dir(name)
    binary = problem_binary(name)
    if not directory.is_dir():
        raise FileNotFoundError(f"missing directory: {directory}")
    if not binary.exists():
        raise FileNotFoundError(f"missing binary: {binary}")


def build_all() -> None:
    subprocess.run(["ninja"], cwd=BUILD_DIR, check=True)


def run_datamaker(name: str) -> pathlib.Path:
    output_path = problem_dir(name) / "output.txt"
    with output_path.open("w") as stdout:
        subprocess.run([str(problem_binary(name))], stdout=stdout, check=True)
    return output_path


def run_solver(name: str, input_path: pathlib.Path) -> pathlib.Path:
    output_path = problem_dir(name) / "output.txt"
    with input_path.open("r") as stdin, output_path.open("w") as stdout:
        subprocess.run(
            [str(problem_binary(name))],
            stdin=stdin,
            stdout=stdout,
            check=True,
        )
    return output_path


def stress_test(main_name: str, good_name: str, datamaker_name: str) -> None:
    iteration = 0
    while True:
        input_path = run_datamaker(datamaker_name)
        main_output = run_solver(main_name, input_path)
        good_output = run_solver(good_name, input_path)

        same = filecmp.cmp(main_output, good_output, shallow=False)
        print(iteration, same)
        if not same:
            break
        iteration += 1


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("main_name")
    parser.add_argument("good_name")
    parser.add_argument("datamaker_name")
    args = parser.parse_args()

    build_all()
    validate_problem(args.main_name)
    validate_problem(args.good_name)
    validate_problem(args.datamaker_name)
    stress_test(args.main_name, args.good_name, args.datamaker_name)


if __name__ == "__main__":
    main()
