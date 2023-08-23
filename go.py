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
        f'touch problem_{name}/{name}.cpp && touch problem_{name}/{name}.in', shell=True)
    with open('./templates/main.cpp', 'r') as f:
        template = f.read()
    # template = template.replace('main.in', f'../problem_{name}/{name}.in')
    # template = template.replace('main.out', f'../problem_{name}/{name}.out')
    with open(f'problem_{name}/{name}.cpp', 'w') as f:
        f.write(template)
    cmake_content = f'''cmake_minimum_required(VERSION 3.15.0)\nproject(problem_{name} VERSION 0.1.0)\nadd_executable(problem_{name} {name}.cpp)
    '''
    with open(f'problem_{name}/CMakeLists.txt', 'w') as f:
        f.write(cmake_content)


def run_problem(names, no_build):
    name = names[0]
    problem = f'problem_{name}'
    if not os.path.exists(f'./{problem}') or not os.path.exists(f'./{problem}/{name}.cpp'):
        init_problem(name)
    infile = names[1]
    if infile != '':
        infile = '<' + f'{problem}/' + infile
    outfile = names[2]
    if outfile != '':
        outfile = '>' + f'{problem}/' + outfile
    subprocess.call(f'mkdir -p build', shell=True)

    # build and run
    make_str = f'cmake .. && make -j8 {problem}'
    if no_build:
        make_str = ':'
    run_str = f'/usr/local/bin/time -v ./build/{problem}/{problem} {infile} {outfile} 2> ./build/{problem}/{problem}_run.txt'
    subprocess.call(
        f'cd build && {make_str} && cd .. && {run_str}', shell=True)

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

    # generate submit_A.cpp
    f = open(f'./{problem}/{name}.cpp', 'r')
    now = f.readlines()
    f.close()
    id = -1
    for i in range(len(now)):
        if 'using namespace std;' in now[i]:
            id = i
            break
    assert id >= 0
    now = now[id + 1:]
    temp = f'./build/{problem}/temp.cpp'
    f = open(temp, 'w')
    for line in now:
        if line.startswith('#include'):
            f.write(line)
    f.close()
    submit = f'{problem}/submit_{name}.cpp'
    subprocess.call(
        f'g++ -std=c++20 -Iutils/system_headers -Itemplates -Iparlaylib/include -E {temp} > {submit}', shell=True)
    f = open('./templates/main.cpp', 'r')
    a = f.readlines()
    f.close()
    id = -1
    for i in range(len(a)):
        if 'using namespace std;' in a[i]:
            id = i
            break
    assert id >= 0
    a = a[:id + 1]
    f = open(f'{submit}', 'r')
    b = f.readlines()
    f.close()
    for i in range(len(b)):
        if re.match('# \d+.*', b[i]) is not None:
            b[i] = '\n'
    b = a + b
    for line in now:
        if not line.startswith('#include'):
            b.append(line)
    f = open(f'{submit}', 'w')
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
    names = args.names
    if 'clean' in names:
        assert names[0] == 'clean'
        print('Enter yes to clean: ', end='')
        confirm = input()
        if confirm == 'yes':
            clean(names[1:])
        else:
            print('Not cleaning. Exit.')
    else:
        while len(names) < 3:
            names.append('')
        run_problem(names, args.no_build)


if __name__ == '__main__':
    main()
