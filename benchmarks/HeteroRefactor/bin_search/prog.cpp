
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

struct __rect_packed_type_LHC8GSSMC1L9R 
{
  int *local0;
  int local1;
  int local2;
  int local3;
  int local4;
  unsigned int _location;
  int _return;
  int _ret0;
  int _ret1;
}
;

int binarySearch(int *arr,int low,int high,int x)
{
  volatile struct __rect_packed_type_LHC8GSSMC1L9R __rect_packed_var_LHC8GSSMC1L9R[1024U];
  volatile unsigned int __rect_packed_top_LHC8GSSMC1L9R = 0U;
  __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _location = 1U;
  __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local0 = arr;
  __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local1 = low;
  __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local2 = high;
  __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local3 = x;
  ++__rect_packed_top_LHC8GSSMC1L9R;
  __rect_func_L0_LHC8GSSMC1L9R:
  if (0U == __rect_packed_top_LHC8GSSMC1L9R) 
    return __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _return;
  --__rect_packed_top_LHC8GSSMC1L9R;
  if (1U == __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _location) 
    goto __rect_func_L1_LHC8GSSMC1L9R;
  if (2U == __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _location) 
    goto __rect_func_L2_LHC8GSSMC1L9R;
  if (3U == __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _location) 
    goto __rect_func_L3_LHC8GSSMC1L9R;
  __rect_func_L1_LHC8GSSMC1L9R:
  if (__rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local2 >= __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local1) {
    __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local4 = __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local1 + (__rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local2 - __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local1) / 2;
// If the element is present at the middle
// itself
    if (__rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local0[__rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local4] == __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local3) {
      __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _return = __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local4;
      goto __rect_func_L0_LHC8GSSMC1L9R;
    }
// If element is smaller than mid, then
// it can only be present in left subarray
    if (__rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local0[__rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local4] > __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local3) {
      if (__rect_packed_top_LHC8GSSMC1L9R + 1U == 1024U) {;
        int __temp0__;
        return __temp0__;
      }
      __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _location = 2U;
      __rect_packed_var_LHC8GSSMC1L9R[1 + __rect_packed_top_LHC8GSSMC1L9R] . local3 = __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local3;
      __rect_packed_var_LHC8GSSMC1L9R[1 + __rect_packed_top_LHC8GSSMC1L9R] . local2 = __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local4 - 1;
      __rect_packed_var_LHC8GSSMC1L9R[1 + __rect_packed_top_LHC8GSSMC1L9R] . local1 = __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local1;
      __rect_packed_var_LHC8GSSMC1L9R[1 + __rect_packed_top_LHC8GSSMC1L9R] . local0 = __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local0;
      ++__rect_packed_top_LHC8GSSMC1L9R;
      __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _location = 1U;
      goto __rect_func_L1_LHC8GSSMC1L9R;
      __rect_func_L2_LHC8GSSMC1L9R:
      __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _ret0 = (__rect_packed_var_LHC8GSSMC1L9R[1 + __rect_packed_top_LHC8GSSMC1L9R] . _return);
      __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _return = __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _ret0;
      goto __rect_func_L0_LHC8GSSMC1L9R;
    }
// Else the element can only be present
// in right subarray
    if (__rect_packed_top_LHC8GSSMC1L9R + 1U == 1024U) {;
      int __temp1__;
      return __temp1__;
    }
    __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _location = 3U;
    __rect_packed_var_LHC8GSSMC1L9R[1 + __rect_packed_top_LHC8GSSMC1L9R] . local3 = __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local3;
    __rect_packed_var_LHC8GSSMC1L9R[1 + __rect_packed_top_LHC8GSSMC1L9R] . local2 = __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local2;
    __rect_packed_var_LHC8GSSMC1L9R[1 + __rect_packed_top_LHC8GSSMC1L9R] . local1 = __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local4 + 1;
    __rect_packed_var_LHC8GSSMC1L9R[1 + __rect_packed_top_LHC8GSSMC1L9R] . local0 = __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . local0;
    ++__rect_packed_top_LHC8GSSMC1L9R;
    __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _location = 1U;
    goto __rect_func_L1_LHC8GSSMC1L9R;
    __rect_func_L3_LHC8GSSMC1L9R:
    __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _ret1 = (__rect_packed_var_LHC8GSSMC1L9R[1 + __rect_packed_top_LHC8GSSMC1L9R] . _return);
    __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _return = __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _ret1;
    goto __rect_func_L0_LHC8GSSMC1L9R;
  }
  __rect_packed_var_LHC8GSSMC1L9R[0 + __rect_packed_top_LHC8GSSMC1L9R] . _return = - 1;
  goto __rect_func_L0_LHC8GSSMC1L9R;
  goto __rect_func_L0_LHC8GSSMC1L9R;
}

int process_top(int n,int x,int arr[1024])
{
  
#pragma HLS INTERFACE port=arr ap_memory depth = 1024
  int result = binarySearch(arr,0,n - 1,x);
  return result;
}
