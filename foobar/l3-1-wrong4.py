def debug(msg):
    #print(msg)
    return

def solution(l):
    totalCount = len(l)
    dividers = [None] * totalCount
    tuples = {}

    i = 0
    while (i < totalCount):
        li = l[i]
        j = i + 1
        while (j < totalCount):
            lj = l[j]
            if (lj >= li and not(lj % li)):
                #debug(str(li) + " divides " + str(lj))
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