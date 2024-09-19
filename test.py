from math import *
import subprocess


def test(A, B, C):
    subprocess.call(f'./go {C} >{C}.out --no-build', shell=True)
    subprocess.call(f'./go {A} <{C}.out >{A}.out --no-build', shell=True)
    subprocess.call(f'./go {B} <{C}.out >{B}.out --no-build', shell=True)
    f1 = open(f'{A}.out', 'r')
    f2 = open(f'{B}.out', 'r')
    s1 = f1.read()
    s2 = f2.read()
    f1.close()
    f2.close()
    if s1 != s2:
        print('error!')
        return False
    return True


if __name__ == '__main__':
    subprocess.call('cd build && cmake .. && make -j8', shell=True)
    for i in range(1000000):
        res = test('A', 'B', 'D')
        print(i, res)
        if not res:
            break
