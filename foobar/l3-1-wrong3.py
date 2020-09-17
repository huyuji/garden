def debug(msg):
    print(msg)

def nextDividendIndex(l, divider, startIndex, endIndex):
    i = startIndex
    while (i < endIndex and l[i] % divider > 0):
        i += 1
    return i

def solution(l):
    length = len(l)
    i = 0
    results = {}
    while (i < length):
        li = l[i]
        j = nextDividendIndex(l, li, i + 1, length)
        while (j < length):
            lj = l[j]
            if (li not in results):
                results[li] = {}
            k = nextDividendIndex(l, lj, j + 1, length)
            while (k < length):
                lk = l[k]
                if (lj not in results[li]):
                    results[li][lj] = {lk}
                else:
                    results[li][lj].add(lk)
                k = nextDividendIndex(l, lj, k + 1, length)
            j = nextDividendIndex(l, li, j + 1, length)
        i += 1
    count = 0
    for li,ljs in results.items():
        for lj,lks in ljs.items():
            count += len(lks)
    return count

print(solution([1,2,3,4,5,6]))
print(solution([1,2,4,8]))
print(solution([1,2,4,4,8]))
print(solution([1,2,4,4,4,8]))
print(solution([1,1,1]))
print(solution([4,2,1]))
print(solution([2]))
print(solution([2,2]))
print(solution([1,1,1,1]))