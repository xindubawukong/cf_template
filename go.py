import argparse
import os
import re
import subprocess
import datetime


def clean():
    folder = str(datetime.datetime.now())
    folder = folder.replace(' ', '_')
    subprocess.call(f'mkdir -p ./.history/{folder}', shell=True)
    for file in os.listdir('.'):
        if not file.endswith('.cpp'):
            continue
        if file == 'main.cpp':
            continue
        subprocess.call(
            f'cp {file} ./.history/{folder}', shell=True)
        subprocess.call(f'rm -rf {file}', shell=True)
    subprocess.call(f'rm -rf ./submit/*', shell=True)
    subprocess.call(f'rm -rf *.in', shell=True)
    subprocess.call(f'rm -rf *.out', shell=True)
    subprocess.call(f'rm -rf in', shell=True)
    subprocess.call(f'touch in', shell=True)


def run_problem(name, no_build):
    if not os.path.exists(f'./{name}.cpp'):
        subprocess.call(f'cp main.cpp {name}.cpp', shell=True)
    subprocess.call(f'mkdir -p build', shell=True)

    # build and run
    make_str = f'cmake .. && make -j8 {name}'
    if no_build:
        make_str = ':'
    run_str = f'/usr/local/bin/time -v ./build/{name} 2> ./build/{name}_run.txt'
    subprocess.call(
        f'cd build && {make_str} && cd .. && {run_str}', shell=True)

    if no_build:
        return

    # print time and memory
    with open(f'./build/{name}_run.txt', 'r') as f:
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
    print(f'[{name}] time = {tt} s, memory = {mm} MB')

    # generate submit_A.cpp
    f = open(f'./{name}.cpp', 'r')
    now = f.readlines()
    f.close()
    id = -1
    for i in range(len(now)):
        if 'using namespace std;' in now[i]:
            id = i
            break
    assert id >= 0
    now = now[id + 1:]
    temp = f'./build/temp.cpp'
    f = open(temp, 'w')
    for line in now:
        if line.startswith('#include'):
            f.write(line)
    f.close()
    submit = f'./submit/submit_{name}.cpp'
    subprocess.call(
        f'g++ -std=c++20 -Iutils/system_headers -Itemplates -Iparlaylib/include -E {temp} > {submit}', shell=True)
    f = open('./main.cpp', 'r')
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
    assert len(names) == 1
    if 'clean' in names:
        print('Enter yes to clean: ', end='')
        confirm = input()
        if confirm == 'yes':
            clean()
        else:
            print('Not cleaning. Exit.')
    else:
        run_problem(names[0], args.no_build)


if __name__ == '__main__':
    main()
