from math import *
import os
from random import *

def make_data():
    pass


def test():
    make_data()
    os.system('cd ../build && ./problem_C && cd ..')
    os.system('cd ../build && ./problem_A && cd ..')
    os.system('cd ../build && ./problem_B && cd ..')
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
    break
