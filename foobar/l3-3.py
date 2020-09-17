import fractions

def solution(m):
    stateCount = len(m)
    totals = [0] * stateCount
    A = []
    I = []
    for i in range (stateCount):
        I.append([])
        A.append([])
        stateTotal = sum(m[i])
        totals[i] = stateTotal
        if (stateTotal == 0):
            for j in range (stateCount):
                if (j == i):
                    A[i].append(fractions.Fraction(1, 1))
                    I[i].append(fractions.Fraction(1, 1))
                else:
                    A[i].append(fractions.Fraction(0, 1))
                    I[i].append(fractions.Fraction(0, 1))
        else:
            for j in range (stateCount):
                if (j == i):
                    A[i].append(fractions.Fraction(stateTotal - m[i][j], stateTotal))
                    I[i].append(fractions.Fraction(1, 1))
                else:
                    A[i].append(fractions.Fraction(-m[i][j], stateTotal))
                    I[i].append(fractions.Fraction(0, 1))

    indices = list(range(stateCount))
    for fd in range(stateCount):
        fdScaler = fractions.Fraction(A[fd][fd].denominator, A[fd][fd].numerator)
        for j in range(stateCount):
            A[fd][j] *= fdScaler
            I[fd][j] *= fdScaler
        for i in indices[0:fd] + indices[fd+1:]: 
            crScaler = A[i][fd]
            for j in range(stateCount): 
                A[i][j] = A[i][j] - crScaler * A[fd][j]
                I[i][j] = I[i][j] - crScaler * I[fd][j]

    lcm = 0
    for i, probability in enumerate(I[0]):
        if (totals[i] == 0):
            if (lcm == 0):
                lcm = probability.denominator
            else:
                lcm = lcm * probability.denominator / fractions.gcd(lcm, probability.denominator)
    results = []
    total = 0
    for i, probability in enumerate(I[0]):
        if (totals[i] == 0):
            value = int(probability.numerator * lcm / probability.denominator)
            results.append(value)
            total += value
    results.append(total)
    return results

print(solution([[0, 2, 1, 0, 0], [0, 0, 0, 3, 4], [0, 0, 0, 0, 0], [0, 0, 0, 0,0], [0, 0, 0, 0, 0]]))
print(solution([[0, 1, 0, 0, 0, 1], [4, 0, 0, 3, 2, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]]))