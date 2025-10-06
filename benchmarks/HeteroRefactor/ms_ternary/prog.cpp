
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

void merge3(int *arr,int left,int mid1,int mid2,int right)
{
  int n1 = mid1 - left + 1;
  int n2 = mid2 - mid1;
  int n3 = right - mid2;
// temporary buffers (size 512 assumes max segment ≤512)
  int A[512];
  int B[512];
  int C[512];
  for (int i = 0; i < n1; i++) 
    A[i] = arr[left + i];
  for (int i = 0; i < n2; i++) 
    B[i] = arr[mid1 + 1 + i];
  for (int i = 0; i < n3; i++) 
    C[i] = arr[mid2 + 1 + i];
  int i = 0;
  int j = 0;
  int k = 0;
  int idx = left;
// 1) while all three have elements, take the smallest of A[i], B[j], C[k]
  while(i < n1 && j < n2 && k < n3){
    if (A[i] <= B[j] && A[i] <= C[k]) {
      arr[idx++] = A[i++];
    }
     else if (B[j] <= A[i] && B[j] <= C[k]) {
      arr[idx++] = B[j++];
    }
     else {
      arr[idx++] = C[k++];
    }
  }
// 2) now merge the remaining two arrays
// 2a) A & B
  while(i < n1 && j < n2){
    arr[idx++] = (A[i] <= B[j]?A[i++] : B[j++]);
  }
// 2b) B & C
  while(j < n2 && k < n3){
    arr[idx++] = (B[j] <= C[k]?B[j++] : C[k++]);
  }
// 2c) A & C
  while(i < n1 && k < n3){
    arr[idx++] = (A[i] <= C[k]?A[i++] : C[k++]);
  }
// 3) copy any leftovers
  while(i < n1)
    arr[idx++] = A[i++];
  while(j < n2)
    arr[idx++] = B[j++];
  while(k < n3)
    arr[idx++] = C[k++];
}
// 3-way mergesort: split into [L..mid1], [mid1+1..mid2], [mid2+1..R]

struct __rect_packed_type_LLXCqpwA0GIoR 
{
  int *local0;
  int local1;
  int local2;
  int local3;
  int local4;
  int local5;
  unsigned int _location;
}
;

void mergeSort3(int *arr,int left,int right)
{
  volatile struct __rect_packed_type_LLXCqpwA0GIoR __rect_packed_var_LLXCqpwA0GIoR[1024U];
  volatile unsigned int __rect_packed_top_LLXCqpwA0GIoR = 0U;
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . _location = 1U;
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local0 = arr;
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local1 = left;
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local2 = right;
  ++__rect_packed_top_LLXCqpwA0GIoR;
  __rect_func_L0_LLXCqpwA0GIoR:
  if (0U == __rect_packed_top_LLXCqpwA0GIoR) 
    return ;
  --__rect_packed_top_LLXCqpwA0GIoR;
  if (1U == __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . _location) 
    goto __rect_func_L1_LLXCqpwA0GIoR;
  if (2U == __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . _location) 
    goto __rect_func_L2_LLXCqpwA0GIoR;
  if (3U == __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . _location) 
    goto __rect_func_L3_LLXCqpwA0GIoR;
  if (4U == __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . _location) 
    goto __rect_func_L4_LLXCqpwA0GIoR;
  __rect_func_L1_LLXCqpwA0GIoR:
  if (__rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local1 >= __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local2) {
    goto __rect_func_L0_LLXCqpwA0GIoR;
  }
// two split-points
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local3 = (__rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local2 - __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local1) / 3;
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local4 = __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local1 + __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local3;
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local5 = __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local1 + 2 * __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local3;
// sort the three chunks
  if (__rect_packed_top_LLXCqpwA0GIoR + 1U == 1024U) {;
    return ;
  }
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . _location = 2U;
  __rect_packed_var_LLXCqpwA0GIoR[1 + __rect_packed_top_LLXCqpwA0GIoR] . local2 = __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local4;
  __rect_packed_var_LLXCqpwA0GIoR[1 + __rect_packed_top_LLXCqpwA0GIoR] . local1 = __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local1;
  __rect_packed_var_LLXCqpwA0GIoR[1 + __rect_packed_top_LLXCqpwA0GIoR] . local0 = __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local0;
  ++__rect_packed_top_LLXCqpwA0GIoR;
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . _location = 1U;
  goto __rect_func_L1_LLXCqpwA0GIoR;
  __rect_func_L2_LLXCqpwA0GIoR:
  0;
  if (__rect_packed_top_LLXCqpwA0GIoR + 1U == 1024U) {;
    return ;
  }
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . _location = 3U;
  __rect_packed_var_LLXCqpwA0GIoR[1 + __rect_packed_top_LLXCqpwA0GIoR] . local2 = __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local5;
  __rect_packed_var_LLXCqpwA0GIoR[1 + __rect_packed_top_LLXCqpwA0GIoR] . local1 = __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local4 + 1;
  __rect_packed_var_LLXCqpwA0GIoR[1 + __rect_packed_top_LLXCqpwA0GIoR] . local0 = __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local0;
  ++__rect_packed_top_LLXCqpwA0GIoR;
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . _location = 1U;
  goto __rect_func_L1_LLXCqpwA0GIoR;
  __rect_func_L3_LLXCqpwA0GIoR:
  0;
  if (__rect_packed_top_LLXCqpwA0GIoR + 1U == 1024U) {;
    return ;
  }
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . _location = 4U;
  __rect_packed_var_LLXCqpwA0GIoR[1 + __rect_packed_top_LLXCqpwA0GIoR] . local2 = __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local2;
  __rect_packed_var_LLXCqpwA0GIoR[1 + __rect_packed_top_LLXCqpwA0GIoR] . local1 = __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local5 + 1;
  __rect_packed_var_LLXCqpwA0GIoR[1 + __rect_packed_top_LLXCqpwA0GIoR] . local0 = __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local0;
  ++__rect_packed_top_LLXCqpwA0GIoR;
  __rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . _location = 1U;
  goto __rect_func_L1_LLXCqpwA0GIoR;
  __rect_func_L4_LLXCqpwA0GIoR:
  0;
// merge all three back together
  merge3(__rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local0,__rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local1,__rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local4,__rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local5,__rect_packed_var_LLXCqpwA0GIoR[0 + __rect_packed_top_LLXCqpwA0GIoR] . local2);
  goto __rect_func_L0_LLXCqpwA0GIoR;
}

int process_top(int n,int *arr)
{
  
#pragma HLS INTERFACE ap_memory port=arr depth=16
  mergeSort3(arr,0,n - 1);
  return 0;
}
