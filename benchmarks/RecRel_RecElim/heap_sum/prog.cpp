#include <algorithm> 
#include <cmath> 
#include <math.h> 

int div_iter_find(int n,int k)
{
  int iter = 0;
  while(n > k - 1){
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
#include "header.hpp"

void heap_sum(int *f,int start,int end)
{
  int size_0 = end - start;
  int size_0_o = end - start;
  int num_iter_size_0 = div_iter_find(size_0,2);
  int num_iter_div;
  num_iter_div = num_iter_size_0;
  int num_iter;
  num_iter = num_iter_div;
  size_0 = std::max(size_0 / pow(2,num_iter + 1),1);
  int init_start = start;
  int curr_iter = 0;
  for (; curr_iter <= num_iter + 0; ++curr_iter) {
    int inner_value = 0;
    for (; inner_value < size_0_o; inner_value = inner_value + size_0) {
      start = inner_value + init_start;
      end = std::min(start + (size_0 - 0),size_0_o);
      if (end - start <= 1) {
        continue; 
      }
       else {
        f[start] += f[start + (end - start) / 2];
      }
    }
    size_0 = size_0 * 2;
  }
}

int process_top(int *arr,int n)
{
  
#pragma HLS INTERFACE ap_memory port=arr depth=16
  heap_sum(arr,0,n);
  return 0;
}
