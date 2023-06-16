#include <stdio.h>

void print_bytes(const unsigned char *bytes, size_t length)
{
  for (size_t i = 0; i < length; ++i)
  {
    printf("%d ", bytes[i]);
  }
  printf("\n");
}