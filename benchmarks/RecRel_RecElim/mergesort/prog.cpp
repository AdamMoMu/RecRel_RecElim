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

void merge(int *arr,int left,int mid,int right)
{
  int n1 = mid - left + 1;
  int n2 = right - mid;
// Create temp vectors
  int L[512];
  int R[512];
// Copy data to temp vectors L[] and R[]
  for (int i = 0; i < n1; i++) 
    L[i] = arr[left + i];
  for (int j = 0; j < n2; j++) 
    R[j] = arr[mid + 1 + j];
  int i = 0;
  int j = 0;
  int k = left;
// Merge the temp vectors back 
// into arr[left..right]
  while(i < n1 && j < n2){
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    }
     else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }
// Copy the remaining elements of L[], 
// if there are any
  while(i < n1){
    arr[k] = L[i];
    i++;
    k++;
  }
// Copy the remaining elements of R[], 
// if there are any
  while(j < n2){
    arr[k] = R[j];
    j++;
    k++;
  }
}
// begin is for left index and end is right index
// of the sub-array of arr to be sorted

void mergeSort(int *arr,int left,int right)
{
  int size_0 = right - left;
  int size_0_o = right - left;
  int num_iter_size_0 = div_iter_find(size_0,2);
  int num_iter_div;
  num_iter_div = num_iter_size_0;
  int num_iter;
  num_iter = num_iter_div;
  size_0 = std::max(size_0 / pow(2,num_iter + 1),1);
  int init_start = left;
  int curr_iter = 0;
  for (; curr_iter <= num_iter + 1; ++curr_iter) {
    int inner_value = 0;
    for (; inner_value < size_0_o; inner_value = inner_value + size_0) {
      left = inner_value + init_start;
      right = std::min(left + (size_0 - 1),size_0_o);
      if (left >= right) {
        continue; 
      }
      int mid = left + size_0 / 2 - 1 - 0;
      merge(arr,left,mid,right);
    }
    size_0 = size_0 * 2;
  }
}

int process_top(int n,int *arr)
{
  
#pragma HLS INTERFACE ap_memory port=arr depth=16
  mergeSort(arr,0,n - 1);
  return 0;
}
