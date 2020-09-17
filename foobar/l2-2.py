def solution(l):    
    total = sum(l)
    l.sort()
    remain = total % 3
    if (remain > 0):
        ones = []
        twos = []
        for index, i in enumerate(l):
            if (i % 3 == 1):
                ones.append(index)
            elif (i % 3 == 2):
                twos.append(index)
        if (remain == 2):
            if (len(twos) > 0):
                del l[twos[0]]
            elif (len(ones) > 1):
                del l[ones[1]]
                del l[ones[0]]
            else:
                return 0
        else: #remain == 1
            if (len(ones) > 0):
                del l[ones[0]]
            elif (len(twos) > 1):
                del l[twos[1]]
                del l[twos[0]]
            else:
                return 0
    if (len(l) == 0):
        return 0
    l.reverse()
    return int(''.join(map(str, l)))

print(solution([3, 1, 4, 1]))
print(solution([3, 1, 4, 1, 5, 9]))
print(solution([0]))
print(solution([1]))
print(solution([2]))
print(solution([3]))
print(int('000'))