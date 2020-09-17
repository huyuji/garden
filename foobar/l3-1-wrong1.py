def debug(s):
    print(s)
    return

def updateCache(cache, dividerIndex, dividendIndex):
    debug ("update cache " + str(dividerIndex) + "-" + str(dividendIndex))
    dividerCache = cache[dividerIndex]
    dividendCache = cache[dividendIndex]
    if (dividerCache is None):
        if (dividendCache is None):
            cache[dividendIndex] = {1: 1}
        else:
            dividendCache[1] += 1
    else: # if divider cache is not None, dividend cache will not be None
        for i, count in dividerCache.items():
            index = i + 1
            if (index in dividendCache):
                dividendCache[index] += count
            else:
                dividendCache[index] = count

factorialCache = {}
def countLuckyTuples(n):
    if (n < 2):
        return 0
    else:
        return n * (n - 1) / 2

def getLuckyTupleCount(n):
    if n not in factorialCache:
        factorialCache[n] = countLuckyTuples(n)
    return factorialCache[n]

# handle same number
def solution(l):
    numberCount = len(l)
    cache = [None] * numberCount
    tripletCount = 0
    for i, currentNumber in enumerate(l):
        debug ("process " + str(i) + "-" + str(currentNumber))
        threshold = currentNumber * 2
        duplicateCount = 0
        for j in range(i + 1, numberCount):
            testNumber = l[j]
            debug ("test against " + str(j) + "-" + str(testNumber))
            if (testNumber < threshold):
                continue
            if (testNumber == currentNumber):
                duplicateCount += 1
                if (duplicateCount == 1):
                    # handle the first duplicate
                    debug ('found 1st duplicate')
                elif (duplicateCount == 2):
                    tripletCount += 1
                continue
            if (testNumber % currentNumber  == 0):
                debug (str(currentNumber) + " divides " + str(testNumber))
                updateCache(cache, i, j)

    total = tripletCount
    for i, cacheValue in enumerate(cache):
        if (cacheValue is not None):
            for j, count in cacheValue.items():
                debug (str(i) + "-" + str(j) + "-" + str(count))
                total += getLuckyTupleCount(j) * count
    return total

print(solution([1,2,3,4,5,6])) # 3
print(solution([1,2,4,8]))     # 4