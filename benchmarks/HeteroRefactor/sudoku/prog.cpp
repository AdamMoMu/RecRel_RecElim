
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

int isValid(int number,int *puzzle,int row,int column)
{
  int i = 0;
  int sectorRow = 3 * (row / 3);
  int sectorCol = 3 * (column / 3);
  int row1 = (row + 2) % 3;
  int row2 = (row + 4) % 3;
  int col1 = (column + 2) % 3;
  int col2 = (column + 4) % 3;
/* Check for the value in the given row and column */
  for (i = 0; i < 9; i++) {
    if (puzzle[i * 9 + column] == number) 
      return false;
    if (puzzle[row * 9 + i] == number) 
      return false;
  }
/* Check the remaining four spaces in this sector */
  if (puzzle[(row1 + sectorRow) * 9 + col1 + sectorCol] == number) 
    return false;
  if (puzzle[(row2 + sectorRow) * 9 + col1 + sectorCol] == number) 
    return false;
  if (puzzle[(row1 + sectorRow) * 9 + col2 + sectorCol] == number) 
    return false;
  if (puzzle[(row2 + sectorRow) * 9 + col2 + sectorCol] == number) 
    return false;
  return true;
}

struct __rect_packed_type_Lz2eOE8GiTZR 
{
  int *local0;
  int local1;
  int local2;
  int local3;
  unsigned int _location;
  bool _return;
  bool _ret0;
  bool _ret1;
  bool _ret2;
  bool _ret3;
}
;

bool sudoku_func(int *puzzle,int row,int column)
{
  volatile struct __rect_packed_type_Lz2eOE8GiTZR __rect_packed_var_Lz2eOE8GiTZR[1024U];
  volatile unsigned int __rect_packed_top_Lz2eOE8GiTZR = 0U;
  __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location = 1U;
  __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local0 = puzzle;
  __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local1 = row;
  __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local2 = column;
  ++__rect_packed_top_Lz2eOE8GiTZR;
  __rect_func_L0_Lz2eOE8GiTZR:
  if (0U == __rect_packed_top_Lz2eOE8GiTZR) 
    return __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _return;
  --__rect_packed_top_Lz2eOE8GiTZR;
  if (1U == __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location) 
    goto __rect_func_L1_Lz2eOE8GiTZR;
  if (2U == __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location) 
    goto __rect_func_L2_Lz2eOE8GiTZR;
  if (3U == __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location) 
    goto __rect_func_L3_Lz2eOE8GiTZR;
  if (4U == __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location) 
    goto __rect_func_L4_Lz2eOE8GiTZR;
  if (5U == __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location) 
    goto __rect_func_L5_Lz2eOE8GiTZR;
  __rect_func_L1_Lz2eOE8GiTZR:
  if (9 == __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local1) {
    __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _return = true;
    goto __rect_func_L0_Lz2eOE8GiTZR;
  }
// Is this element already set?  If so, we don't want to
// change it.  Recur immediately to the next cell.
  if (__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local0[__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local1 * 9 + __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local2]) {
    if (__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local2 == 8) {
      if (__rect_packed_top_Lz2eOE8GiTZR + 1U == 1024U) {;
        bool __temp0__;
        return __temp0__;
      }
      __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location = 2U;
      __rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . local2 = 0;
      __rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . local1 = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local1 + 1;
      __rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . local0 = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local0;
      ++__rect_packed_top_Lz2eOE8GiTZR;
      __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location = 1U;
      goto __rect_func_L1_Lz2eOE8GiTZR;
      __rect_func_L2_Lz2eOE8GiTZR:
      __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _ret0 = (__rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . _return);
      __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _return = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _ret0;
      goto __rect_func_L0_Lz2eOE8GiTZR;
    }
     else {
      if (__rect_packed_top_Lz2eOE8GiTZR + 1U == 1024U) {;
        bool __temp1__;
        return __temp1__;
      }
      __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location = 3U;
      __rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . local2 = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local2 + 1;
      __rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . local1 = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local1;
      __rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . local0 = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local0;
      ++__rect_packed_top_Lz2eOE8GiTZR;
      __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location = 1U;
      goto __rect_func_L1_Lz2eOE8GiTZR;
      __rect_func_L3_Lz2eOE8GiTZR:
      __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _ret1 = (__rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . _return);
      __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _return = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _ret1;
      goto __rect_func_L0_Lz2eOE8GiTZR;
    }
  }
  for (__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local3 = 1; __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local3 < 10; __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local3++) {
    if ((isValid(__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local3,__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local0,__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local1,__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local2))) {
// Temporarily bind this entry to nextNum
      __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local0[__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local1 * 9 + __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local2] = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local3;
      if (__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local2 == 8) {
        if (__rect_packed_top_Lz2eOE8GiTZR + 1U == 1024U) {;
          bool __temp2__;
          return __temp2__;
        }
        __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location = 4U;
        __rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . local2 = 0;
        __rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . local1 = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local1 + 1;
        __rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . local0 = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local0;
        ++__rect_packed_top_Lz2eOE8GiTZR;
        __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location = 1U;
        goto __rect_func_L1_Lz2eOE8GiTZR;
        __rect_func_L4_Lz2eOE8GiTZR:
        __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _ret2 = (__rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . _return);
        if (__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _ret2) {
          __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _return = true;
          goto __rect_func_L0_Lz2eOE8GiTZR;
        }
      }
       else {
        if (__rect_packed_top_Lz2eOE8GiTZR + 1U == 1024U) {;
          bool __temp3__;
          return __temp3__;
        }
        __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location = 5U;
        __rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . local2 = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local2 + 1;
        __rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . local1 = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local1;
        __rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . local0 = __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local0;
        ++__rect_packed_top_Lz2eOE8GiTZR;
        __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _location = 1U;
        goto __rect_func_L1_Lz2eOE8GiTZR;
        __rect_func_L5_Lz2eOE8GiTZR:
        __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _ret3 = (__rect_packed_var_Lz2eOE8GiTZR[1 + __rect_packed_top_Lz2eOE8GiTZR] . _return);
        if (__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _ret3) {
          __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _return = true;
          goto __rect_func_L0_Lz2eOE8GiTZR;
        }
      }
// We failed to find a valid value for this, undo
      __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local0[__rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local1 * 9 + __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . local2] = 0;
    }
  }
// Have to backtrack
  __rect_packed_var_Lz2eOE8GiTZR[0 + __rect_packed_top_Lz2eOE8GiTZR] . _return = false;
  goto __rect_func_L0_Lz2eOE8GiTZR;
  goto __rect_func_L0_Lz2eOE8GiTZR;
}

int process_top(int *puzzle,int row,int column)
{
  
#pragma HLS INTERFACE ap_memory port = puzzle depth=81
  sudoku_func(puzzle,row,column);
  return 0;
}
