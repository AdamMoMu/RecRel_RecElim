
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

struct __rect_packed_type_ackerman___Fb_i_Gb__i__sep__i__Fe_ 
{
  int local0;
  int local1;
  unsigned int _location;
  int _return;
  int _ret0;
  int _ret1;
  int _ret2;
}
;

int ackerman(int m,int n)
{
  volatile struct __rect_packed_type_ackerman___Fb_i_Gb__i__sep__i__Fe_ __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[1024U];
  volatile unsigned int __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_ = 0U;
  __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _location = 1U;
  __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local0 = m;
  __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local1 = n;
  ++__rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_;
  __rect_func_L0_ackerman___Fb_i_Gb__i__sep__i__Fe_:
  if (0U == __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_) 
    return __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _return;
  --__rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_;
  if (1U == __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _location) 
    goto __rect_func_L1_ackerman___Fb_i_Gb__i__sep__i__Fe_;
  if (2U == __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _location) 
    goto __rect_func_L2_ackerman___Fb_i_Gb__i__sep__i__Fe_;
  if (3U == __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _location) 
    goto __rect_func_L3_ackerman___Fb_i_Gb__i__sep__i__Fe_;
  if (4U == __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _location) 
    goto __rect_func_L4_ackerman___Fb_i_Gb__i__sep__i__Fe_;
  __rect_func_L1_ackerman___Fb_i_Gb__i__sep__i__Fe_:
  if (__rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local0 == 0) {
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _return = __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local1 + 1;
    goto __rect_func_L0_ackerman___Fb_i_Gb__i__sep__i__Fe_;
  }
   else if (__rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local0 > 0 && __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local1 == 0) {
    if (__rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_ + 1U == 1024U) {;
      int __temp0__;
      return __temp0__;
    }
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _location = 2U;
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[1 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local1 = 1;
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[1 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local0 = __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local0 - 1;
    ++__rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_;
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _location = 1U;
    goto __rect_func_L1_ackerman___Fb_i_Gb__i__sep__i__Fe_;
    __rect_func_L2_ackerman___Fb_i_Gb__i__sep__i__Fe_:
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _ret0 = (__rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[1 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _return);
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _return = __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _ret0;
    goto __rect_func_L0_ackerman___Fb_i_Gb__i__sep__i__Fe_;
  }
   else {
    if (__rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_ + 1U == 1024U) {;
      int __temp1__;
      return __temp1__;
    }
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _location = 3U;
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[1 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local1 = __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local1 - 1;
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[1 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local0 = __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local0;
    ++__rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_;
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _location = 1U;
    goto __rect_func_L1_ackerman___Fb_i_Gb__i__sep__i__Fe_;
    __rect_func_L3_ackerman___Fb_i_Gb__i__sep__i__Fe_:
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _ret1 = (__rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[1 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _return);
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local1 = __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _ret1;
    if (__rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_ + 1U == 1024U) {;
      int __temp2__;
      return __temp2__;
    }
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _location = 4U;
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[1 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local1 = __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local1;
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[1 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local0 = __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . local0 - 1;
    ++__rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_;
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _location = 1U;
    goto __rect_func_L1_ackerman___Fb_i_Gb__i__sep__i__Fe_;
    __rect_func_L4_ackerman___Fb_i_Gb__i__sep__i__Fe_:
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _ret2 = (__rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[1 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _return);
    __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _return = __rect_packed_var_ackerman___Fb_i_Gb__i__sep__i__Fe_[0 + __rect_packed_top_ackerman___Fb_i_Gb__i__sep__i__Fe_] . _ret2;
    goto __rect_func_L0_ackerman___Fb_i_Gb__i__sep__i__Fe_;
  }
  goto __rect_func_L0_ackerman___Fb_i_Gb__i__sep__i__Fe_;
}

int process_top(int m,int n)
{
  return ackerman(m,n);
}
