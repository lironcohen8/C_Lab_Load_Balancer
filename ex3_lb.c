#include "lb.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int main()
{
  srand(time(NULL));
  load_balancer_t lb = {0};
  setup_load_balancer(&lb);

  while (true) {
  load_balancer_handle_client(&lb);
  }

  return 0;
}