from math import *
import os
from random import *


def make_data():
    lines = []
    n = 6
    lines.append(str(n) + '\n')
    for i in range(n):
        s = ''
        for j in range(4):
            s += str(randint(0, 10)) + ' '
        s += '\n'
        lines.append(s)
    f = open('problem_A/A.in', 'w')
    f.writelines(lines)
    f.close()


def test():
    make_data()
    os.system('cd build && ./problem_A')
    os.system('cd build && ./problem_B')
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


for i in range(1000):
    res = test()
    print(res)
    if not res:
        break
