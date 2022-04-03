from math import *
import os
from random import *

def make_data():
    pass


def test():
    make_data()
    os.system('cd build && ./main && cd ..')
    os.system('cd build && ./biaosuan && cd ..')
    f1 = open('output.txt', 'r')
    f2 = open('output2.txt', 'r')
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
    if not res:
        break
