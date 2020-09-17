import fractions

def solution(m):
    stateCount = len(m)
    endStateFlags = [False] * stateCount
    stateProbabilities = m[0]
    denominator = sum(m[0])

    fromStateIndex = 1
    while (fromStateIndex < stateCount):
        fromResults = m[fromStateIndex]
        total = sum(fromResults)
        if (total == 0):
            endStateFlags[fromStateIndex] = True
        else:
            toStateIndex = fromStateIndex + 1
            while (toStateIndex < stateCount):
                stateProbabilities[toStateIndex] = fromResults[toStateIndex] * stateProbabilities[fromStateIndex] + total * stateProbabilities[toStateIndex]
                toStateIndex += 1
            denominator = denominator * total
        fromStateIndex += 1
    
    endStateProbabilities = []
    gcd = 0
    for stateIndex, probability in enumerate(stateProbabilities):
        if (endStateFlags[stateIndex]):
            endStateProbabilities.append(probability)
            if (gcd == 0):
                gcd = probability
            else:
                gcd = fractions.gcd(gcd, probability)
    results = []
    for probability in endStateProbabilities:
        results.append(int(probability/gcd))
    results.append(sum(results))
    return results

print(solution([[0, 2, 1, 0, 0], [0, 0, 0, 3, 4], [0, 0, 0, 0, 0], [0, 0, 0, 0,0], [0, 0, 0, 0, 0]]))
print(solution([[0, 1, 0, 0, 0, 1], [4, 0, 0, 3, 2, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]]))

Test 1 passed!
Test 2 passed!
Test 3 failed  [Hidden]
Test 4 failed  [Hidden]
Test 5 failed  [Hidden]
Test 6 failed  [Hidden]
Test 7 failed  [Hidden]
Test 8 failed  [Hidden]
Test 9 failed  [Hidden]
Test 10 passed! [Hidden]