
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

struct __rect_packed_type_LIDSfWiNN392R 
{
  int *local0;
  int local1;
  int local2;
  int local3;
  int local4;
  int local5;
  int local6;
  int local7;
  unsigned int _location;
}
;

void quickSort(int *vec,int low,int high)
{
  volatile struct __rect_packed_type_LIDSfWiNN392R __rect_packed_var_LIDSfWiNN392R[1024U];
  volatile unsigned int __rect_packed_top_LIDSfWiNN392R = 0U;
  __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . _location = 1U;
  __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0 = vec;
  __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local1 = low;
  __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local2 = high;
  ++__rect_packed_top_LIDSfWiNN392R;
  __rect_func_L0_LIDSfWiNN392R:
  if (0U == __rect_packed_top_LIDSfWiNN392R) 
    return ;
  --__rect_packed_top_LIDSfWiNN392R;
  if (1U == __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . _location) 
    goto __rect_func_L1_LIDSfWiNN392R;
  if (2U == __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . _location) 
    goto __rect_func_L2_LIDSfWiNN392R;
  if (3U == __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . _location) 
    goto __rect_func_L3_LIDSfWiNN392R;
  __rect_func_L1_LIDSfWiNN392R:
  if (__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local1 < __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local2) {
    __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local3 = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0[__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local2];
    __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local4 = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local1 - 1;
    for (__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local5 = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local1; __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local5 <= __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local2 - 1; __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local5++) {
      if (__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0[__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local5] <= __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local3) {
        __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local4++;
        __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local6 = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0[__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local4];
        __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0[__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local4] = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0[__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local5];
        __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0[__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local5] = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local6;
      }
    }
    __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local7 = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0[__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local4 + 1];
    __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0[__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local4 + 1] = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0[__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local2];
    __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0[__rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local2] = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local7;
    __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local3 = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local4 + 1;
    if (__rect_packed_top_LIDSfWiNN392R + 1U == 1024U) {;
      return ;
    }
    __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . _location = 2U;
    __rect_packed_var_LIDSfWiNN392R[1 + __rect_packed_top_LIDSfWiNN392R] . local2 = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local3 - 1;
    __rect_packed_var_LIDSfWiNN392R[1 + __rect_packed_top_LIDSfWiNN392R] . local1 = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local1;
    __rect_packed_var_LIDSfWiNN392R[1 + __rect_packed_top_LIDSfWiNN392R] . local0 = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0;
    ++__rect_packed_top_LIDSfWiNN392R;
    __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . _location = 1U;
    goto __rect_func_L1_LIDSfWiNN392R;
    __rect_func_L2_LIDSfWiNN392R:
    0;
    if (__rect_packed_top_LIDSfWiNN392R + 1U == 1024U) {;
      return ;
    }
    __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . _location = 3U;
    __rect_packed_var_LIDSfWiNN392R[1 + __rect_packed_top_LIDSfWiNN392R] . local2 = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local2;
    __rect_packed_var_LIDSfWiNN392R[1 + __rect_packed_top_LIDSfWiNN392R] . local1 = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local3 + 1;
    __rect_packed_var_LIDSfWiNN392R[1 + __rect_packed_top_LIDSfWiNN392R] . local0 = __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . local0;
    ++__rect_packed_top_LIDSfWiNN392R;
    __rect_packed_var_LIDSfWiNN392R[0 + __rect_packed_top_LIDSfWiNN392R] . _location = 1U;
    goto __rect_func_L1_LIDSfWiNN392R;
    __rect_func_L3_LIDSfWiNN392R:
    0;
  }
  goto __rect_func_L0_LIDSfWiNN392R;
}

int process_top(int n,int arr[16])
{
  
#pragma HLS INTERFACE port=arr ap_memory depth = 16
  quickSort(arr,0,n - 1);
  return 0;
}
