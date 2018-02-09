#include <iostream>
#include <algorithm>
#include "utility.h"

namespace
{
  void find_longest_ascending_sub_series(int* array, int size)
  {
    int* count = new int[size];

    for (unsigned int i = 0; i < size; i++)
    {
      count[i] = 1;
      for (unsigned int j = 0; j < i; j++)
      {
        if (array[i] > array[j] && count[j] + 1 > count[i])
        {
          count[i] = count[j] + 1;
        }
      }
    }

    for (unsigned int i = 0; i < size; i++)
    {
      std::cout << count[i] << " ";
    }
    std::cout << std::endl;
  }
}

void test_find_longest_ascending_sub_series()
{
  int size = 10;
  int* array = new int[size];
  for(unsigned int i = 0; i < size; i++)
  {
    array[i] = i;
  }

  for (unsigned int i=size - 1; i > 0; --i) {
    swap (array, i, random(0, i));
  }

  for(unsigned int i = 0; i < size; i++)
  {
    std::cout << array[i] << " ";
  }
  std::cout << std::endl;

  find_longest_ascending_sub_series(array, size);
}