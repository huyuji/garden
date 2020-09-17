def isFactor(li, lj, factors):
    if (lj not in factors):
        factors[lj] = {1,lj}
        for i in range(2, int(lj**0.5) + 1):
            factor = int(lj / i)
            if (factor * i == lj):
                factors[lj].add(i)
                factors[lj].add(factor)
    return li in factors[lj]

def solution(l):
    totalCount = len(l)
    factors = {}
    dividers = [None] * totalCount
    tuples = {}

    i = 0
    while (i < totalCount):
        li = l[i]
        threshold = li * 2
        j = i + 1
        while (j < totalCount):
            lj = l[j]
            if (li == lj or (lj >= threshold and isFactor(li, lj, factors))):
                if (dividers[j] is None):
                    dividers[j] = {li}
                else:
                    dividers[j].add(li)

                if (dividers[i] is not None):
                    dividerSet = dividers[i]
                    if (lj not in tuples):
                        tuples[lj] = {li: dividerSet.copy()}
                    elif (li not in tuples[lj]):
                        tuples[lj][li] = dividerSet.copy()
                    else:
                        tuples[lj][li].update(dividerSet) 
            j += 1
        i += 1

    count = 0
    for li,ljs in tuples.items():
        for lj,lks in ljs.items():
            count += len(lks)
    return count 

print(solution([1,2,3,4,5,6]))  #3
print(solution([1,2,4,8]))      #4
print(solution([1,2,4,4,8]))    #7
print(solution([1,2,4,4,4,8]))  #8
print(solution([1,1,1]))        #1
print(solution([4,2,1]))        #0
print(solution([2]))            #0
print(solution([2,2]))          #0
print(solution([1,1,1,1]))      #1