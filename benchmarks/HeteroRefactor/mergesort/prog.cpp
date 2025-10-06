
struct __dst_alloc_list_base_t 
{
  unsigned int prev;
  unsigned int next;
}
;
// === BEGIN REC SUPPORT LIBRARY ===
#include <stdio.h>
#include <stdlib.h>
const char *__dst_filename = "";
unsigned long long __dst_file = 0;
// === END REC SUPPORT LIBRARY ===
// #include "header.hpp"

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

struct __rect_packed_type_L9iK38hOsq2fR 
{
  int *local0;
  int local1;
  int local2;
  int local3;
  unsigned int _location;
}
;

void mergeSort(int *arr,int left,int right)
{
  volatile struct __rect_packed_type_L9iK38hOsq2fR __rect_packed_var_L9iK38hOsq2fR[1024U];
  volatile unsigned int __rect_packed_top_L9iK38hOsq2fR = 0U;
  __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . _location = 1U;
  __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local0 = arr;
  __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local1 = left;
  __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local2 = right;
  ++__rect_packed_top_L9iK38hOsq2fR;
  __rect_func_L0_L9iK38hOsq2fR:
  if (0U == __rect_packed_top_L9iK38hOsq2fR) 
    return ;
  --__rect_packed_top_L9iK38hOsq2fR;
  if (1U == __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . _location) 
    goto __rect_func_L1_L9iK38hOsq2fR;
  if (2U == __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . _location) 
    goto __rect_func_L2_L9iK38hOsq2fR;
  if (3U == __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . _location) 
    goto __rect_func_L3_L9iK38hOsq2fR;
  __rect_func_L1_L9iK38hOsq2fR:
  if (__rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local1 >= __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local2) {
    goto __rect_func_L0_L9iK38hOsq2fR;
  }
  __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local3 = __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local1 + (__rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local2 - __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local1) / 2;
  if (__rect_packed_top_L9iK38hOsq2fR + 1U == 1024U) {;
    return ;
  }
  __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . _location = 2U;
  __rect_packed_var_L9iK38hOsq2fR[1 + __rect_packed_top_L9iK38hOsq2fR] . local2 = __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local3;
  __rect_packed_var_L9iK38hOsq2fR[1 + __rect_packed_top_L9iK38hOsq2fR] . local1 = __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local1;
  __rect_packed_var_L9iK38hOsq2fR[1 + __rect_packed_top_L9iK38hOsq2fR] . local0 = __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local0;
  ++__rect_packed_top_L9iK38hOsq2fR;
  __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . _location = 1U;
  goto __rect_func_L1_L9iK38hOsq2fR;
  __rect_func_L2_L9iK38hOsq2fR:
  0;
  if (__rect_packed_top_L9iK38hOsq2fR + 1U == 1024U) {;
    return ;
  }
  __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . _location = 3U;
  __rect_packed_var_L9iK38hOsq2fR[1 + __rect_packed_top_L9iK38hOsq2fR] . local2 = __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local2;
  __rect_packed_var_L9iK38hOsq2fR[1 + __rect_packed_top_L9iK38hOsq2fR] . local1 = __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local3 + 1;
  __rect_packed_var_L9iK38hOsq2fR[1 + __rect_packed_top_L9iK38hOsq2fR] . local0 = __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local0;
  ++__rect_packed_top_L9iK38hOsq2fR;
  __rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . _location = 1U;
  goto __rect_func_L1_L9iK38hOsq2fR;
  __rect_func_L3_L9iK38hOsq2fR:
  0;
  merge(__rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local0,__rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local1,__rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local3,__rect_packed_var_L9iK38hOsq2fR[0 + __rect_packed_top_L9iK38hOsq2fR] . local2);
  goto __rect_func_L0_L9iK38hOsq2fR;
}

int process_top(int n,int *arr)
{
  
#pragma HLS INTERFACE ap_memory port=arr depth=16
  mergeSort(arr,0,n - 1);
  return 0;
}
