import fractions

def solution(m):
    stateCount = len(m)
    totals = [0] * stateCount

    i = 0
    while (i < stateCount):
        m[i][i] = 0
        totals[i] = sum(m[i])
        i += 1
    stateProbabilities = m[0]
    denominator = totals[0]

    i = 1
    while (i < stateCount):
        probabilities = m[i]
        total = totals[i]
        if (total > 0):
            j = 1
            while (j < stateCount):
                if (j > i or totals[j] == 0):
                    stateProbabilities[j] = int(probabilities[j] * stateProbabilities[i] + total * stateProbabilities[j])
                j += 1
            denominator = int(denominator * total)
        i += 1
    
    endStateProbabilities = []
    gcd = 0
    for stateIndex, probability in enumerate(stateProbabilities):
        if (totals[stateIndex] == 0):
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

print(solution([[0, 2, 1, 0, 0], [0, 0, 0, 3, 4], [0, 0, 0, 0, 0], [0, 0, 0, 0, 0], [0, 0, 0, 0, 0]]))
print(solution([[0, 2, 1, 0, 0], [0, 0, 0, 0, 0], [0, 0, 0, 3, 4], [0, 0, 0, 0, 0], [0, 0, 0, 0, 0]]))
print(solution([[0, 2, 1, 0, 0], [0, 0, 0, 0, 0], [0, 0, 0, 0, 0], [0, 0, 0, 3, 4], [0, 0, 0, 0, 0]]))
print(solution([[0, 1, 0, 0, 0, 1], [4, 0, 0, 3, 2, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]]))
