#include <iostream>
#include <vector>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& numbers, int target) {
        int begin = 0;
        int end = numbers.size() - 1;
        while (begin < end) {
            int sum = numbers[begin] + numbers[end];
            if (sum > target) {
                do {
                    --end;
                    sum = numbers[begin] + numbers[end];
                } while (sum > target);
            } else if (sum < target) {
                do {
                    sum = numbers[begin] + numbers[end];
                } while (sum < target && ++end < numbers.size());
            }

            if (sum == target) {
                return {begin + 1, end + 1};
            } else {
                ++begin;
                if (end == numbers.size()) {
                    --end;
                }
            }
        }
        return {0, 0};
    }
};

void test(std::vector<int> numbers, int target) {
    auto result = (new Solution)->twoSum(numbers, target);
    std::cout << result[0] << " " << result[1] << "\n";
}

int main() {
    test({2,7,11,15}, 9);
    test({2,3,4}, 6);
    test({-1,0}, -1);
}
