def solution(l):
    totalCount = len(l)
    dividerCount = [0] * totalCount
    tupleCount = 0

    i = 0
    while (i < totalCount):
        li = l[i]
        threshold = li * 2 - 1
        j = i + 1
        while (j < totalCount):
            lj = l[j]
            if (lj == li or (lj > threshold and not(lj % li))):
                 dividerCount[j] += 1
                 tupleCount += dividerCount[i]
            j += 1
        i += 1
    return tupleCount

print(solution([1,2,3,4,5,6]))  #3
print(solution([1,2,4,8]))      #4
print(solution([1,2,4,4,8]))    #7
print(solution([1,2,4,4,4,8]))  #8
print(solution([1,1,1]))        #1
print(solution([4,2,1]))        #0
print(solution([2]))            #0
print(solution([2,2]))          #0
print(solution([1,1,1,1]))      #1