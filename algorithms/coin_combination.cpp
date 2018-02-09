#include "utility.h"

namespace
{
  static unsigned int *result = 0;
  static unsigned int result_count = 0;

  void print_result(unsigned int end_index)
  {
    result_count++;
    for (unsigned int i = 0; i <= end_index; i += 2)
    {
      if (i)
      {
        std::cout << " + ";
      }
      std::cout << result[i] << " * " << result[i + 1];
    }
    std::cout << std::endl;
  }

  void coin_combination_recursive(unsigned int *values, unsigned int size, unsigned int total, unsigned int result_index)
  {
    if (!result)
    {
      result = new unsigned int[2 * size];
    }

    if (size == 1)
    {
      if (total % values[0] == 0)
      {
        result[result_index] = values[0];
        result[result_index + 1] = total / values[0];
        print_result(result_index);
      }

      return;
    }

    for (unsigned int i = 0; i < size; i++)
    {
      result[result_index] = values[i];
      unsigned int max = total / values[i];
      unsigned int j = max;

      if (total % values[i] == 0)
      {
        result[result_index + 1] = j;
        print_result(result_index);
        j--;
      }

      for (; j > 0; j--)
      {
        result[result_index + 1] = j;
        coin_combination_recursive(values + i + 1, size - i - 1, total - j * values[i], result_index + 2);
      }
    }
  }
}

void test_coin_combination_recursive()
{
  unsigned int values[] = {15, 23, 29, 41, 67};
  coin_combination_recursive(values, 5, 1808, 0);

  std::cout << std::endl << result_count << std::endl;
}