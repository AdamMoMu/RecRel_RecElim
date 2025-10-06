#include "header.hpp"
#include <math.h> 

int factorial(int n)
{
  int recursion_array[1] = {0};
  int num_iter_n = n / 1;
  int num_iter_sub;
  num_iter_sub = num_iter_n;
  int num_iter;
  num_iter = num_iter_sub;
  n = n - num_iter * 1;
  int curr_iter = 0;
  for (; curr_iter <= num_iter; ++curr_iter) {
    while(true){
      if (n == 0 || n - 1 == 0) {
        recursion_array[0] = 1;
        break; 
      }
      recursion_array[0] = n * recursion_array[1 - 1];
      break; 
    }
    n = n + 1;
  }
  return recursion_array[0];
}

int process_top(int n)
{
  return factorial(n);
}
