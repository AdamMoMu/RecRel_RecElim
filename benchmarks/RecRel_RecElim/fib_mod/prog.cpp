#include <math.h> 

int div_iter_find(int n,int k)
{
  int iter = 0;
  while(n > 1){
    n = n / k;
    iter = iter + 1;
  }
  return iter;
}

int pow(int n,int k)
{
  int prod = 1;
  for (int i = 0; i < k; ++i) {
    prod = prod * n;
  }
  return prod;
}

void array_shift(int *arr,int n)
{
  for (int i = 1; i < n; ++i) {
    arr[i - 1] = arr[i];
  }
}
#include "header.hpp"

int fib(int n,int m)
{
  int recursion_array[2] = {0};
  int num_iter_m = m / 1;
  int num_iter_n = div_iter_find(n,2);
  int num_iter_sub;
  num_iter_sub = num_iter_m;
  int num_iter_div;
  num_iter_div = num_iter_n;
  int num_iter;
  if (num_iter_sub < num_iter_div) {
    num_iter = num_iter_sub;
    m = m - num_iter * 1;
    n = n / pow(2,num_iter);
  }
   else {
    num_iter = num_iter_div;
    m = m - num_iter * 1;
    int temp_m = m - 1;
    n = n / pow(2,num_iter);
    int temp_n = n / 2;
    array_shift(recursion_array,2);
    recursion_array[2 - 1] = 1;
  }
  int curr_iter = 0;
  for (; curr_iter <= num_iter; ++curr_iter) {
    while(true){
      if (n <= 1 || m <= 1) {
        int temp_return = 1;
        array_shift(recursion_array,2);
        recursion_array[2 - 1] = temp_return;
        break; 
      }
      int temp_return = recursion_array[2 - 1] + recursion_array[2 - 2];
      array_shift(recursion_array,2);
      recursion_array[2 - 1] = temp_return;
      break; 
    }
    m = m + 1;
    n = n * 2;
  }
  return recursion_array[1];
}

int process_top(int n,int m)
{
  return fib(n,m);
}
