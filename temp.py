from datetime import datetime


def get_date(dd, mm, yy):
    try:
        return datetime.strptime('20' + yy + mm + dd, '%Y%m%d')
    except:
        return None


def check(date, born):
    return (born.year + 18, born.month, born.day) <= (date.year, date.month, date.day)


def main():
    dd, mm, yy = input().split('.')
    date = get_date(dd, mm, yy)
    a = input().split('.')
    for i, j, k in [(0,1,2),(0,2,1),(1,0,2),(1,2,0),(2,0,1),(2,1,0)]:
        bd = a[i]
        bm = a[j]
        by = a[k]
        born = get_date(bd, bm, by)
        if born != None:
            if check(date, born):
                print('YES')
                return
    print('NO')


if __name__ == '__main__':
    main()
