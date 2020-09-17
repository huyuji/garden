def debug(msg):
    print(msg)

def solution(l):
    totalCount = len(l)
    dividerCount = [0] * totalCount
    duplicateFlag = [False] * totalCount
    tripletCount = 0

    # handle duplicates
    i = 0
    while (i < totalCount):
        j = i + 1
        while (j < totalCount and l[j] == l[i]):
            l[j] = 0
            j += 1
        duplicateCount = j - i - 1
        if (duplicateCount > 0):
            duplicateFlag[i] = True
            #debug("Found " + str(duplicateCount) + " duplicate of " + str(l[i]))
        if (duplicateCount > 1):
            tripletCount += 1
        i = j

    for i, currentNumber in enumerate(l):
        if (currentNumber == 0):
            continue
        if (duplicateFlag[i]):
            tripletCount += dividerCount[i]
        threshold = currentNumber * 2
        for j in range(i + 1, totalCount):
            testNumber = l[j]
            if (testNumber == 0 or testNumber < threshold):
                continue
            if (testNumber % currentNumber == 0):
                dividerCount[j] += 1
                tripletCount += dividerCount[i]
                if (duplicateFlag[i]):
                    tripletCount += 1
    return tripletCount

print(solution([1,2,3,4,5,6]))
print(solution([1,2,4,8]))
print(solution([1,2,4,4,8]))
print(solution([1,2,4,4,4,8]))
print(solution([1,1,1]))
print(solution([4,2,1]))