
#include <math.h> 

void array_shift(int *arr,int n)
{
  for (int i = 1; i < n; ++i) {
    arr[i - 1] = arr[i];
  }
}
#include "header.hpp"

int fib(int n)
{
  int recursion_array[2] = {0};
  int num_iter_n = n / 1;
  int num_iter_sub;
  num_iter_sub = num_iter_n;
  int num_iter;
  num_iter = num_iter_sub;
  n = n - num_iter * 1;
  int curr_iter = 0;
  for (; curr_iter <= num_iter; ++curr_iter) {
    while(true){
      if (n <= 1) {
        int temp_return = 1;
        array_shift(recursion_array,2);
        recursion_array[2 - 1] = temp_return;
        break; 
      }
      int temp_return = recursion_array[2 - 2] + recursion_array[2 - 1];
      array_shift(recursion_array,2);
      recursion_array[2 - 1] = temp_return;
      break; 
    }
    n = n + 1;
  }
  return recursion_array[1];
}

int process_top(int n)
{
  return fib(n);
}
