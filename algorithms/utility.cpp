#include "utility.h"

void swap(int* array, int i, int j)
{
  int temp = array[i];
  array[i] = array[j];
  array[j] = temp;
}

int random(int start, int end)
{
  return rand() % (end - start + 1) + start;
}
