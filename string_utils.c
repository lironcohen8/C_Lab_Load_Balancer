#include "string_utils.h"
#include <string.h>

unsigned int count_sub_string_appearances(char* string_to_search, const char* sub_string)
{
  unsigned int count = 0;
  char* tmp = string_to_search;
  while ((tmp = strstr(tmp, sub_string))) {
    count++;
    tmp++;
  }

  return count;
}