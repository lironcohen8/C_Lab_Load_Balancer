#include "memory_utils.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int realloc_larger_buffer(char** buffer, int original_buffer_size, int new_buffer_size)
{
  assert(new_buffer_size > original_buffer_size);
  *buffer = (char*)realloc(*buffer, new_buffer_size);
  assert(*buffer != NULL);
  memset(*buffer + original_buffer_size, 0, new_buffer_size - original_buffer_size);
  return new_buffer_size;
}