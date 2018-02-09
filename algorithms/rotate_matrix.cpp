#include "utility.h"

namespace
{
#define SIZE 5

  static void print_matrix(unsigned int matrix[SIZE][SIZE], unsigned int dimension)
  {
    for (unsigned int i = 0; i < dimension; i++)
    {
      for (unsigned int j = 0; j < dimension; j++)
      {
        std::cout << matrix[i][j] << "\t";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  void rotate_matrix(unsigned int matrix[SIZE][SIZE], unsigned int dimension)
  {
    for (unsigned int i = 0; i < dimension / 2; i++)
    {
      unsigned int j = dimension - i - 1;
      for (unsigned int k = 0; k < j - i; k++)
      {
        unsigned int temp = matrix[i][i + k];
        matrix[i][i + k] = matrix[i + k][j];
        matrix[i + k][j] = matrix[j][j - k];
        matrix[j][j - k] = matrix[j - k][i];
        matrix[j - k][i] = temp;
      }
    }
  }
}

void test_rotate_matrix()
{
  unsigned int matrix[SIZE][SIZE] = {
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5},
  };

  print_matrix(matrix, SIZE);
  rotate_matrix(matrix, SIZE);
  print_matrix(matrix, SIZE);
}