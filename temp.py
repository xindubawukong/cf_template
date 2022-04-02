import subprocess

PIPE = subprocess.PIPE

p2 = subprocess.Popen('python3 ./build/local_testing_tool.py', shell=True, stdin=PIPE, stdout=PIPE, stderr=PIPE)
p1 = subprocess.Popen('./build/main', shell=True, stdin=PIPE, stdout=PIPE, stderr=PIPE)
