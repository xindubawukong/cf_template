
def add_edge(lines, x, y):
    lines.append(str(x) + ' ' + str(y) + '\n')

with open('input.txt', 'w') as f:
    n = 10000
    lines = [str(n) + '\n']
    add_edge(lines, 1, 2)
    add_edge(lines,1, 3)
    add_edge(lines,1, 4)
    left = range(5, n + 1)
    num = len(left) // 3
    for t in left[0:num]:
        add_edge(lines,2, t)
    for t in left[num:2 * num]:
        add_edge(lines,3, t)
    for t in left[num * 2:]:
        add_edge(lines,4, t)
    f.writelines(lines)