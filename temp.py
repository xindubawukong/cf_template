with open('input.txt', 'w') as f:
    n = 200
    lines = [str(n) + '\n']
    for i in range(199):
        line = str(i + 1) + ' ' + str(i + 2) + '\n'
        lines.append(line)
    f.writelines(lines)