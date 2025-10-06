
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

struct __rect_packed_type_fib___Fb_i_Gb__i__Fe_ 
{
  int local0;
  unsigned int _location;
  int _return;
  int _ret0;
  int _ret1;
}
;

int fib(int n)
{
  volatile struct __rect_packed_type_fib___Fb_i_Gb__i__Fe_ __rect_packed_var_fib___Fb_i_Gb__i__Fe_[1024U];
  volatile unsigned int __rect_packed_top_fib___Fb_i_Gb__i__Fe_ = 0U;
  __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _location = 1U;
  __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . local0 = n;
  ++__rect_packed_top_fib___Fb_i_Gb__i__Fe_;
  __rect_func_L0_fib___Fb_i_Gb__i__Fe_:
  if (0U == __rect_packed_top_fib___Fb_i_Gb__i__Fe_) 
    return __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _return;
  --__rect_packed_top_fib___Fb_i_Gb__i__Fe_;
  if (1U == __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _location) 
    goto __rect_func_L1_fib___Fb_i_Gb__i__Fe_;
  if (2U == __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _location) 
    goto __rect_func_L2_fib___Fb_i_Gb__i__Fe_;
  if (3U == __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _location) 
    goto __rect_func_L3_fib___Fb_i_Gb__i__Fe_;
  __rect_func_L1_fib___Fb_i_Gb__i__Fe_:
  if (__rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . local0 <= 1) {
    __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _return = 1;
    goto __rect_func_L0_fib___Fb_i_Gb__i__Fe_;
  }
  if (__rect_packed_top_fib___Fb_i_Gb__i__Fe_ + 1U == 1024U) {;
    int __temp0__;
    return __temp0__;
  }
  __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _location = 2U;
  __rect_packed_var_fib___Fb_i_Gb__i__Fe_[1 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . local0 = __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . local0 - 2;
  ++__rect_packed_top_fib___Fb_i_Gb__i__Fe_;
  __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _location = 1U;
  goto __rect_func_L1_fib___Fb_i_Gb__i__Fe_;
  __rect_func_L2_fib___Fb_i_Gb__i__Fe_:
  __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _ret0 = (__rect_packed_var_fib___Fb_i_Gb__i__Fe_[1 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _return);
  if (__rect_packed_top_fib___Fb_i_Gb__i__Fe_ + 1U == 1024U) {;
    int __temp1__;
    return __temp1__;
  }
  __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _location = 3U;
  __rect_packed_var_fib___Fb_i_Gb__i__Fe_[1 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . local0 = __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . local0 - 1;
  ++__rect_packed_top_fib___Fb_i_Gb__i__Fe_;
  __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _location = 1U;
  goto __rect_func_L1_fib___Fb_i_Gb__i__Fe_;
  __rect_func_L3_fib___Fb_i_Gb__i__Fe_:
  __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _ret1 = (__rect_packed_var_fib___Fb_i_Gb__i__Fe_[1 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _return);
  __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _return = __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _ret0 + __rect_packed_var_fib___Fb_i_Gb__i__Fe_[0 + __rect_packed_top_fib___Fb_i_Gb__i__Fe_] . _ret1;
  goto __rect_func_L0_fib___Fb_i_Gb__i__Fe_;
  goto __rect_func_L0_fib___Fb_i_Gb__i__Fe_;
}

int process_top(int n)
{
  return fib(n);
}
