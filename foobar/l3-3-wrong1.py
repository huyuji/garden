import fractions

def solution(m):
    stateCount = len(m)
    endStateFlags = [False] * stateCount
    stateProbabilities = [None] * stateCount
    i = 0
    while(i < stateCount):
        m[i][i] = 0
        stateProbabilities[i] = fractions.Fraction(0, 1)
        i += 1
    stateProbabilities[0] = fractions.Fraction(1, 1)

    for fromStateIndex, fromStateResults in enumerate(m):
        total = sum(fromStateResults)
        if (total == 0):
            endStateFlags[fromStateIndex] = True
            continue
        toStateIndex = 1
        while (toStateIndex < stateCount):
            if (toStateIndex > fromStateIndex or endStateFlags[toStateIndex]):
                stateProbabilities[toStateIndex] += fractions.Fraction(fromStateResults[toStateIndex], total) * stateProbabilities[fromStateIndex]
            toStateIndex += 1
    
    endStateProbabilities = []
    lcm = 0
    for stateIndex, probability in enumerate(stateProbabilities):
        if (endStateFlags[stateIndex]):
            endStateProbabilities.append(probability)
            if (lcm == 0):
                lcm = int(probability.denominator)
            else:
                lcm = int(lcm * probability.denominator / fractions.gcd(lcm, probability.denominator))
    results = []
    for probability in endStateProbabilities:
        results.append(int(probability.numerator * lcm / probability.denominator))
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
Test 9 passed! [Hidden]
Test 10 failed  [Hidden]