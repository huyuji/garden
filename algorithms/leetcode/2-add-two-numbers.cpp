#include <iostream>
#include <vector>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* result = nullptr;
        ListNode* p1 = l1;
        ListNode* p2 = l2;
        int carry = 0;
        while (p1 && p2) {
            int sum = p1->val + p2->val + carry;
            if (sum > 9) {
                sum -= 10;
                carry = 1;
            } else {
                carry = 0;
            }
            p1->val = sum;
            p2->val = sum;

            if (p1->next == nullptr && p2->next == nullptr) {
                if (carry) {
                    p1->next = new ListNode(carry);
                }
                return l1;
            }

            p1 = p1->next;
            p2 = p2->next;
        }

        result = p1 ? l1 : l2;

        ListNode* p = p1 ? p1 : p2;
        while (p) {
            int sum = p->val + carry;
            if (sum > 9) {
                sum -= 10;
                carry = 1;
            } else {
                carry = 0;
            }
            p->val = sum;

            if (p->next == nullptr) {
                if (carry) {
                    p->next = new ListNode(carry);
                }
                return result;
            }
            p = p->next;
        }

        return result;
    };
};

ListNode* createList(const std::vector<int> & digits) {
    ListNode* head = new ListNode(digits[0]);
    ListNode* l = head;
    for (unsigned int i = 1; i < digits.size(); i++) {
        l->next = new ListNode(digits[i]);
        l = l->next;
    }
    return head;
}

bool testList(ListNode* result, const std::vector<int> & expected) {
    int index = 0;
    while (result) {
        if (result->val != expected[index]) {
            return false;
        }

        result = result->next;
        index++;
    }

    return index == expected.size();
}

void printList(ListNode* l) {
    while (l) {
        std::cout << l->val << " ";
        l = l->next;
    }
    std::cout << std::endl;
}

void test(const std::vector<int> & input1, const std::vector<int> & input2, const std::vector<int> & expected) {
    ListNode* l1 = createList(input1);
    printList(l1);
    ListNode* l2 = createList(input2);
    printList(l2);
    ListNode* result = (new Solution())->addTwoNumbers(l1, l2);
    printList(result);
    std::cout << testList(result, expected) << "\n\n";
}

int main()
{
    test({2, 4 ,3}, {5, 6, 4}, {7, 0, 8});
    test({0}, {0}, {0});
    test({9, 9, 9, 9, 9, 9, 9}, {9, 9, 9, 9}, {8, 9, 9, 9, 0, 0, 0, 1});
    return 0;
}
