if [ -z $1 ]; then
  echo "Please specify problem name or 'clean'"
  exit 0
fi

if [ $1 == "clean" ]; then
  python3 go.py --clean
  exit 0
fi

python3 go.py --problem $1
