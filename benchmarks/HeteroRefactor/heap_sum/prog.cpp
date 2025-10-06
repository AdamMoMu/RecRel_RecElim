
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

struct __rect_packed_type_LGB6VO9rDPG2R 
{
  int *local0;
  int local1;
  int local2;
  unsigned int _location;
}
;

void heap_sum(int *f,int start,int end)
{
  volatile struct __rect_packed_type_LGB6VO9rDPG2R __rect_packed_var_LGB6VO9rDPG2R[1024U];
  volatile unsigned int __rect_packed_top_LGB6VO9rDPG2R = 0U;
  __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . _location = 1U;
  __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local0 = f;
  __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local1 = start;
  __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local2 = end;
  ++__rect_packed_top_LGB6VO9rDPG2R;
  __rect_func_L0_LGB6VO9rDPG2R:
  if (0U == __rect_packed_top_LGB6VO9rDPG2R) 
    return ;
  --__rect_packed_top_LGB6VO9rDPG2R;
  if (1U == __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . _location) 
    goto __rect_func_L1_LGB6VO9rDPG2R;
  if (2U == __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . _location) 
    goto __rect_func_L2_LGB6VO9rDPG2R;
  if (3U == __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . _location) 
    goto __rect_func_L3_LGB6VO9rDPG2R;
  __rect_func_L1_LGB6VO9rDPG2R:
  if (__rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local2 - __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local1 <= 1) {
    goto __rect_func_L0_LGB6VO9rDPG2R;
  }
   else {
    if (__rect_packed_top_LGB6VO9rDPG2R + 1U == 1024U) {;
      return ;
    }
    __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . _location = 2U;
    __rect_packed_var_LGB6VO9rDPG2R[1 + __rect_packed_top_LGB6VO9rDPG2R] . local2 = __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local1 + (__rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local2 - __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local1) / 2;
    __rect_packed_var_LGB6VO9rDPG2R[1 + __rect_packed_top_LGB6VO9rDPG2R] . local1 = __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local1;
    __rect_packed_var_LGB6VO9rDPG2R[1 + __rect_packed_top_LGB6VO9rDPG2R] . local0 = __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local0;
    ++__rect_packed_top_LGB6VO9rDPG2R;
    __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . _location = 1U;
    goto __rect_func_L1_LGB6VO9rDPG2R;
    __rect_func_L2_LGB6VO9rDPG2R:
    0;
    if (__rect_packed_top_LGB6VO9rDPG2R + 1U == 1024U) {;
      return ;
    }
    __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . _location = 3U;
    __rect_packed_var_LGB6VO9rDPG2R[1 + __rect_packed_top_LGB6VO9rDPG2R] . local2 = __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local2;
    __rect_packed_var_LGB6VO9rDPG2R[1 + __rect_packed_top_LGB6VO9rDPG2R] . local1 = __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local1 + (__rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local2 - __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local1) / 2;
    __rect_packed_var_LGB6VO9rDPG2R[1 + __rect_packed_top_LGB6VO9rDPG2R] . local0 = __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local0;
    ++__rect_packed_top_LGB6VO9rDPG2R;
    __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . _location = 1U;
    goto __rect_func_L1_LGB6VO9rDPG2R;
    __rect_func_L3_LGB6VO9rDPG2R:
    0;
    __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local0[__rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local1] += __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local0[__rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local1 + (__rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local2 - __rect_packed_var_LGB6VO9rDPG2R[0 + __rect_packed_top_LGB6VO9rDPG2R] . local1) / 2];
  }
  goto __rect_func_L0_LGB6VO9rDPG2R;
}

int process_top(int *arr,int n)
{
  
#pragma HLS INTERFACE ap_memory port=arr depth=16
  heap_sum(arr,0,n);
  return 0;
}
