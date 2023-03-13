from math import *
import subprocess


def test():
    subprocess.call('./go C A B --no-build', shell=True)
    f1 = open('problem_A/A.out', 'r')
    f2 = open('problem_B/B.out', 'r')
    s1 = f1.read()
    s2 = f2.read()
    f1.close()
    f2.close()
    if s1 != s2:
        print('error!')
        return False
    return True


if __name__ == '__main__':
    subprocess.call('cd build && cmake .. && make', shell=True)
    for i in range(1000):
        res = test()
        print(i, res)
        if not res:
            break
