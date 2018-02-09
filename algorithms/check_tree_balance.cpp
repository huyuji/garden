#include "utility.h"
#include <algorithm>

namespace
{
  struct Node
  {
    int data;
    Node* left;
    Node* right;

    Node() : data(0), left(0), right(0) {};
  };

  int check_tree_balance(const Node* root)
  {
    int left = root->left ? check_tree_balance(root->left) : 0;
    if (left < 0)
    {
      return -1;
    }

    int right = root->right ? check_tree_balance(root->right) : 0;
    if (right < 0)
    {
      return -1;
    }

    if (left == 0 && right == 0)
    {
      return 1;
    }
    else if (left - right > 1 || right - left > 1)
    {
      return -1;
    }
    else
    {
      return std::max(left, right) + 1;
    }
  }
}

void test_check_tree_balance()
{
  Node root;
  root.left = new Node();
  root.left->left = new Node();
  root.right = new Node();

  std::cout << (check_tree_balance(&root) != -1) << std::endl;
}