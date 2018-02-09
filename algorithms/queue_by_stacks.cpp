#include "utility.h"
#include <stack>

namespace
{
  class QueueByStack
  {
  public:
    void in(int i)
    {
      newStack.push(i);
    }

    void out()
    {
      if (oldStack.size() == 0)
      {
        transfer();
      }

      oldStack.pop();
    }

    int peek()
    {
      if (oldStack.size() == 0)
      {
        transfer();
      }

      return oldStack.top();
    }

    int size()
    {
      return oldStack.size() + newStack.size();
    }

  private:
    void transfer()
    {
      while (newStack.size())
      {
        int temp = newStack.top();
        newStack.pop();
        oldStack.push(temp);
      }
    }

    std::stack<int> newStack;
    std::stack<int> oldStack;
  };
}

void test_queue_by_stack()
{
  QueueByStack q;
  q.in(1);
  q.in(2);
  q.in(3);
  std::cout << "size: " << q.size() << std::endl;
  std::cout << q.peek() << " ";
  q.out();
  std::cout << q.peek() << " ";
  q.out();
  std::cout << std::endl;
  std::cout << "size: " << q.size() << std::endl;
  q.in(4);
  q.in(5);
  std::cout << std::endl;
  std::cout << "size: " << q.size() << std::endl;
  while(q.size())
  {
    std::cout << q.peek() << " ";
    q.out();
  }
  std::cout << std::endl;
  std::cout << "size: " << q.size() << std::endl;
}