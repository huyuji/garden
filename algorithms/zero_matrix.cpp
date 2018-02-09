#include "utility.h"

namespace
{
#define M 5
#define N 4

  static void print_matrix(unsigned int matrix[M][N])
  {
    for (unsigned int i = 0; i < M; i++)
    {
      for (unsigned int j = 0; j < N; j++)
      {
        std::cout << matrix[i][j] << "\t";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  void zero_matrix(unsigned int matrix[M][N], unsigned int m, unsigned int n)
  {
    bool *rowFlags = new bool[m];
    memset(rowFlags, 1, sizeof(bool)*m);

    bool *columnFlags = new bool[n];
    memset(columnFlags, 1, sizeof(bool)*n);

    for (unsigned int i = 0; i < m; i++)
    {
      for (unsigned int j = 0; j < n; j++)
      {
        if (matrix[i][j] == 0)
        {
          rowFlags[i] = 0;
          columnFlags[j] = 0;
        }
      }
    }

    for (unsigned int i = 0; i < m; i++)
    {
      if (!rowFlags[i])
      {
        for (unsigned int j = 0; j < n; j++)
        {
          matrix[i][j] = 0;
        }
      }
    }

    for (unsigned int i = 0; i < n; i++)
    {
      if (!columnFlags[i])
      {
        for (unsigned int j = 0; j < m; j++)
        {
          matrix[j][i] = 0;
        }
      }
    }
  }
}

void test_zero_matrix()
{
    unsigned int matrix[M][N] =
    {
        {1, 2, 3, 4},
        {1, 2, 3, 4},
        {1, 2, 0, 0},
        {1, 2, 3, 4},
        {1, 2, 3, 4},
    };

    print_matrix(matrix);
    std::cout << std::endl;

    zero_matrix(matrix, M, N);
    print_matrix(matrix);
}