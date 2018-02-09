#include "utility.h"

namespace
{
  void compress_string(const char *str, unsigned int size)
  {
    char *compressed = new char[2 * size];
    char c = str[0];
    unsigned int count = 1;
    unsigned int pos = 0;

    for (unsigned int i = 1; i < size; i++)
    {
      if (c == str[i])
      {
        count++;
      }
      else
      {
        compressed[pos++] = c;
        compressed[pos++] = count;
        c = str[i];
        count = 1;
      }
    }
    compressed[pos++] = c;
    compressed[pos++] = count;

    std::cout << "compressed: ";
    for (unsigned int i = 0; i < pos; i += 2)
    {
      std::cout << compressed[i] << (int)compressed[i + 1];
    }
  }
}

void test_compress_string()
{
  compress_string("aabccccaaa", 10);
}