def solution(x, y):
    i = x * (x + 1) / 2
    j = 0
    if (y > 1):
        j = (2 * x + y - 2) * (y - 1) / 2
    return str(int(i + j))

print(solution(3, 2))
print(solution(2, 3))
print(solution(5, 10))
print(solution(100000, 100000))