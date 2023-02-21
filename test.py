from math import *
import subprocess

def test():
    subprocess.call('cd build && ./problem_C', shell=True)
    subprocess.call('cd build && ./problem_A', shell=True)
    subprocess.call('cd build && ./problem_B', shell=True)
    f1 = open('problem_A/A.out', 'r')
    f2 = open('problem_B/B.out', 'r')
    s1 = f1.read()
    s2 = f2.read()
    f1.close()
    f2.close()
    t1 = float(s1)
    t2 = float(s2)
    print(t1, t2)
    if abs(t1 - t2) / abs(t1) > 1e-3:
        print('error!')
        return False
    return True


if __name__ == '__main__':
    subprocess.call('cd build && cmake .. && make', shell=True)

    for i in range(1000):
        res = test()
        print(res)
        if not res:
            break
