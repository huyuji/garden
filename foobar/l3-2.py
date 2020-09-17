def solution(l):
    binary = bin(int(l))
    length = len(binary)
    totalCount = 0
    oneCount = 0
    i = length - 1
    while (i > 2):
        if (binary[i] == '0'):
            if (oneCount == 0):
                totalCount += 1
            elif (oneCount == 1):
                totalCount += 3
                oneCount = 0
            else:
                totalCount += oneCount + 1
                oneCount = 1
        else:
            oneCount += 1
        i -= 1
    if (oneCount == 1):
        totalCount += 2
    elif (oneCount > 0):
        totalCount += oneCount + 2
    return totalCount

for testNumber in ['8', '9', '10', '11', '12', '13', '14', '15', '16', '30']:
    print(solution(testNumber))
print(solution('1500000000000000000000000000000000'))