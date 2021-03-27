#include <vector>
#include <iostream>

using namespace std;

class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;
        while (left < right) {
            unsigned int middle = (left + right) / 2;
            if (nums[middle] == target) {
                return middle;
            } else if (nums[middle] < target) {
                left = middle + 1;
            } else {
                right = middle - 1;
            }
        }

        if (left == right && nums[left] == target) {
            return left;
        } else {
            return -1;
        }
    }
};

void test(std::vector<int> nums, int target) {
    std::cout << (new Solution())->search(nums, target) << "\n";
}

int main() {
    test({-1,0,3,5,9,12}, 9);
    test({-1,0,3,5,9,12}, 2);
    test({5}, 5);
    test({5}, 3);
}
