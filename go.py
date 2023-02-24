import argparse
import os
import subprocess
import datetime


def init_problem(name):
    subprocess.call(f'mkdir -p problem_{name}', shell=True)
    subprocess.call(
        f'touch problem_{name}/{name}.cpp && touch problem_{name}/{name}.in && touch problem_{name}/{name}.out', shell=True)
    with open('main.cpp', 'r') as f:
        template = f.read()
    template = template.replace('debug.h', '../debug.h')
    template = template.replace('main.in', f'../problem_{name}/{name}.in')
    template = template.replace('main.out', f'../problem_{name}/{name}.out')
    with open(f'problem_{name}/{name}.cpp', 'w') as f:
        f.write(template)
    cmake_content = f'''
        cmake_minimum_required(VERSION 3.15.0)
        project(problem_{name} VERSION 0.1.0)

        add_executable(problem_{name} {name}.cpp)
    '''
    with open(f'problem_{name}/CMakeLists.txt', 'w') as f:
        f.write(cmake_content)


def run_problem(name):
    if not os.path.exists(f'./problem_{name}'):
        init_problem(name)
    subprocess.call(f'mkdir -p build', shell=True)
    subprocess.call(
        f'cd build && cmake .. && make && ./problem_{name}/problem_{name}', shell=True)


def clean():
    folder = str(datetime.datetime.now())
    folder = folder.replace(' ', '_')
    subprocess.call(f'mkdir -p ./.history/{folder}', shell=True)
    subprocess.call(f'cp -r problem_* ./.history/{folder}', shell=True)
    subprocess.call(f'rm -rf problem_*', shell=True)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--problem', type=str, help='the problem to run')
    parser.add_argument('--clean', action='store_true', help='clean ALL.')
    args = parser.parse_args()
    if args.clean:
        print('Enter yes to clean: ', end='')
        confirm = input()
        if confirm == 'yes':
            clean()
        else:
            print('Not cleaning. Exit.')
    elif args.problem is not None:
        run_problem(args.problem)
    else:
        parser.print_help()


if __name__ == '__main__':
    main()
