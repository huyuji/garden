long minTime(vector<int> files, int numCores, int limit) {
    vector<int> savedCosts(limit);
    long totalCost = 0;
    for (int lineCount : files)
    {
        if (lineCount % numCores == 0)
        {
            int save = lineCount - lineCount / numCores;
            if (save > savedCosts[0])
            {
                totalCost += savedCosts[0] + lineCount / numCores;
                savedCosts[0] = save;
                int i = 0;
                while (2 * i < limit - 1)
                {
                    int leftIndex = 2 * i + 1;
                    int rightIndex = 2 * i + 2;
                    int smallIndex = savedCosts[leftIndex] < savedCosts [rightIndex] ? leftIndex : rightIndex;
                    if (savedCosts[i] > savedCosts[leftIndex] || savedCosts[i] > savedCosts[rightIndex])
                    {
                        int temp = savedCosts[i];
                        savedCosts[i] = savedCosts[smallIndex];
                        savedCosts[smallIndex] = temp;
                        i = smallIndex;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                totalCost += lineCount;
            }
        }
        else
        {
            totalCost += lineCount;
        }
    }
    return totalCost;
}