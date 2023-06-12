import argparse
import os
import re
import subprocess
import datetime


def clean(names):
    if len(names) == 0:
        names = ['*']
    folder = str(datetime.datetime.now())
    folder = folder.replace(' ', '_')
    subprocess.call(f'mkdir -p ./.history/{folder}', shell=True)
    for name in names:
        subprocess.call(
            f'cp -r problem_{name} ./.history/{folder}', shell=True)
        subprocess.call(f'rm -rf problem_{name}', shell=True)


def init_problem(name):
    subprocess.call(f'mkdir -p problem_{name}', shell=True)
    subprocess.call(
        f'touch problem_{name}/{name}.cpp && touch problem_{name}/{name}.in && touch problem_{name}/{name}.out', shell=True)
    with open('main.cpp', 'r') as f:
        template = f.read()
    template = template.replace('main.in', f'../problem_{name}/{name}.in')
    template = template.replace('main.out', f'../problem_{name}/{name}.out')
    with open(f'problem_{name}/{name}.cpp', 'w') as f:
        f.write(template)
    cmake_content = f'''
        cmake_minimum_required(VERSION 3.15.0)
        project(problem_{name} VERSION 0.1.0)

        add_executable(problem_{name} {name}.cpp)
        if(${{XdbwkCfTemplate_IncludeParlay}})
            target_link_libraries(problem_{name} PUBLIC parlay)
        endif()
    '''
    with open(f'problem_{name}/CMakeLists.txt', 'w') as f:
        f.write(cmake_content)


def run_problem(names, no_build):
    for name in names:
        if not os.path.exists(f'./problem_{name}') or not os.path.exists(f'./problem_{name}/{name}.cpp'):
            init_problem(name)
    subprocess.call(f'mkdir -p build', shell=True)
    problems = list(map(lambda name: f'problem_{name}', names))
    make_str = 'cmake .. && make -j8 ' + ' '.join(problems)
    if no_build:
        make_str = ':'
    run_str = ' && '.join(
        list(map(lambda problem: f'/usr/local/bin/time -v ./{problem}/{problem} 2> ./{problem}/{problem}_run.txt', problems)))
    subprocess.call(f'cd build && {make_str} && {run_str}', shell=True)
    for name in names:
        problem = f'problem_{name}'

        # print time and memory
        if not no_build:
            with open(f'./build/{problem}/{problem}_run.txt', 'r') as f:
                timev = f.readlines()
            tt = 0
            mm = 0
            bad = ''
            for line in timev:
                if line.find('User time') != -1:
                    tt = float(line.split(' ')[-1])
                if line.find('Maximum resident set size') != -1:
                    mm = float(line.split(' ')[-1]) / 1000
                if line.find('Command terminated by signal') != -1:
                    bad = line
            if len(bad) > 0:
                for line in timev:
                    if not line.startswith('\t'):
                        print(line.replace('\n', ''))
            print(f'[{problem}] time = {tt} s, memory = {mm} MB')

        # generate problem_A_submit.cpp
        submit = f'{problem}/submit_{problem}.cpp'
        subprocess.call(
            f'g++ -std=c++20 -Iutils/system_headers -Itemplates -Iparlaylib/include -E {problem}/{name}.cpp > {submit}', shell=True)
        f = open('main.cpp', 'r')
        a = f.readlines()
        f.close()
        f = open(f'{submit}', 'r')
        b = f.readlines()
        f.close()
        f = open(f'{submit}', 'w')
        for t in a:
            if t.startswith('#include <'):
                f.write(t)
        f.write('\n')
        for i in range(len(b)):
            if re.match('# \d+.*', b[i]) is not None:
                b[i] = '\n'
        for i in range(len(b)):
            if i < len(b) - 1 and b[i] == '\n' and b[i + 1] == '\n':
                continue
            f.write(b[i])
        f.close()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('names', nargs='+')
    parser.add_argument('--no-build', action='store_true')
    args = parser.parse_args()
    if 'clean' in args.names:
        assert args.names[0] == 'clean'
        print('Enter yes to clean: ', end='')
        confirm = input()
        if confirm == 'yes':
            clean(args.names[1:])
        else:
            print('Not cleaning. Exit.')
    else:
        run_problem(args.names, args.no_build)


if __name__ == '__main__':
    main()
