
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

struct __rect_packed_type_L3bUcPadlSAUR 
{
  int *local0;
  int local1;
  unsigned int _location;
  int _return;
  int _ret0;
}
;

int sum_list(int *arr,int n)
{
  volatile struct __rect_packed_type_L3bUcPadlSAUR __rect_packed_var_L3bUcPadlSAUR[1024U];
  volatile unsigned int __rect_packed_top_L3bUcPadlSAUR = 0U;
  __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . _location = 1U;
  __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . local0 = arr;
  __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . local1 = n;
  ++__rect_packed_top_L3bUcPadlSAUR;
  __rect_func_L0_L3bUcPadlSAUR:
  if (0U == __rect_packed_top_L3bUcPadlSAUR) 
    return __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . _return;
  --__rect_packed_top_L3bUcPadlSAUR;
  if (1U == __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . _location) 
    goto __rect_func_L1_L3bUcPadlSAUR;
  if (2U == __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . _location) 
    goto __rect_func_L2_L3bUcPadlSAUR;
  __rect_func_L1_L3bUcPadlSAUR:
  if (__rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . local1 <= 1) {
    __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . _return = __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . local0[0];
    goto __rect_func_L0_L3bUcPadlSAUR;
  }
  if (__rect_packed_top_L3bUcPadlSAUR + 1U == 1024U) {;
    int __temp0__;
    return __temp0__;
  }
  __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . _location = 2U;
  __rect_packed_var_L3bUcPadlSAUR[1 + __rect_packed_top_L3bUcPadlSAUR] . local1 = __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . local1 - 1;
  __rect_packed_var_L3bUcPadlSAUR[1 + __rect_packed_top_L3bUcPadlSAUR] . local0 = __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . local0;
  ++__rect_packed_top_L3bUcPadlSAUR;
  __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . _location = 1U;
  goto __rect_func_L1_L3bUcPadlSAUR;
  __rect_func_L2_L3bUcPadlSAUR:
  __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . _ret0 = (__rect_packed_var_L3bUcPadlSAUR[1 + __rect_packed_top_L3bUcPadlSAUR] . _return);
  __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . _return = __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . local0[__rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . local1 - 1] + __rect_packed_var_L3bUcPadlSAUR[0 + __rect_packed_top_L3bUcPadlSAUR] . _ret0;
  goto __rect_func_L0_L3bUcPadlSAUR;
  goto __rect_func_L0_L3bUcPadlSAUR;
}

int process_top(int *arr,int n)
{
  
#pragma HLS INTERFACE ap_memory port=arr depth=16
  return sum_list(arr,n);
}
