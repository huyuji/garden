#include <iostream>
#include <vector>

class Solution {
public:
    int maxSubArray(const std::vector<int> & nums)
    {
        int maxSum = nums[0];
        int i = 0;

        while (i < nums.size() && nums[i] <= 0)
        {
            if (nums[i] > maxSum)
            {
                maxSum = nums[i];
            }
            ++i;
        }
        if (i == nums.size())
        {
            return maxSum;
        }

        maxSum = 0;
        int sum = 0;
        while (i < nums.size())
        {
            int positiveSum = 0;
            while (i < nums.size() && nums[i] >=0)
            {
                positiveSum += nums[i];
                ++i;
            }

            int negativeSum = 0;
            while (i < nums.size() && nums[i] <= 0)
            {
                negativeSum += nums[i];
                ++i;
            }

            if (sum >= 0)
            {
                sum += positiveSum;
                if (sum > maxSum)
                {
                    maxSum = sum;
                }
            }
            else
            {
                sum = positiveSum;
                if (positiveSum > maxSum)
                {
                    maxSum = positiveSum;
                }
            }
            sum += negativeSum;
        }

        return maxSum;
    }
};

void test(const std::vector<int> & nums)
{
    std::cout << (new Solution())->maxSubArray(nums) << "\n";
}

int main()
{
    test({-2,1,-3,4,-1,2,1,-5,4});
    test({1});
    test({5,4,-1,7,8});
    test({1,-2,0});
    test({8,-19,5,-4,20});
    test({0});
}
