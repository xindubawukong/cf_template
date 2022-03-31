
import random

def add_edge(lines, x, y):
    lines.append(str(x) + ' ' + str(y) + '\n')

with open('input.txt', 'w') as f:
    n = 100000
    lines = [str(n) + '\n']
    for i in range(2, n + 1, 1):
        add_edge(lines, i, random.randint(1, i - 1))
    f.writelines(lines)