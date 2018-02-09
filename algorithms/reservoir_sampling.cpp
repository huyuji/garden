#include <stdlib.h>
#include <iostream>
#include "utility.h"

namespace
{
  void reservoir_sampling(int* array, int size, int sampleCount)
  {
    for (int i = sampleCount; i < size; i++)
    {
      int temp = random(0, i);
      if (temp < sampleCount)
      {
        swap(array, temp, i);
      }
    }
  }
}

void test_reservoir_sampling()
{
  for(int j = 0; j < 10; j++)
  {
    int size = 10;
    int* array = new int[size];
    for(int i = 0; i < size; i++)
    {
      array[i] = i;
    }

    int sampleCount = 3;

    reservoir_sampling(array, size, sampleCount);

    for(int i = 0; i < sampleCount; i++)
    {
      std::cout << array[i] << " ";
    }

    std::cout << std::endl;
  }
}