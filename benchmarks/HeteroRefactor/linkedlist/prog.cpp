
struct __dst_alloc_list_base_t 
{
  unsigned int prev;
  unsigned int next;
}
;
// type __didxtypedef_NODE__typedef_declaration was ::NODE*
typedef unsigned long long __didxtypedef_NODE__typedef_declaration;
// === BEGIN REC SUPPORT LIBRARY ===
#include <stdio.h>
#include <stdlib.h>
const char *__dst_filename = "";
unsigned long long __dst_file = 0;
// === END REC SUPPORT LIBRARY ===
// #include "header.hpp"
#include <stdio.h>
#include <stdlib.h>
bool g_fallback = false;
typedef struct {
int info;}DATA;
typedef struct node {
DATA data;
__didxtypedef_NODE__typedef_declaration next;}NODE;

struct __dst_alloc_list__dmemtypedef_NODE__typedef_declaration 
{
  struct __dst_alloc_list_base_t _link;
  NODE _data;
}
;
struct __dst_alloc_list__dmemtypedef_NODE__typedef_declaration __dmemtypedef_NODE__typedef_declaration[1025U];
# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 54 "<stdin>"
typedef unsigned long long __dst_alloc_size_t;
typedef unsigned char __dst_alloc_bucket_size_t;
# 73 "<stdin>"
unsigned char __dst_alloc_node_split__dmemtypedef_NODE__typedef_declaration[(1 << (10)) / 8];
static unsigned char __dst_alloc_test_parent_split__dmemtypedef_NODE__typedef_declaration(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    return (__dst_alloc_node_split__dmemtypedef_NODE__typedef_declaration[index / 8] >> (index % 8)) & 1;
}
static void __dst_alloc_flip_parent_split__dmemtypedef_NODE__typedef_declaration(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemtypedef_NODE__typedef_declaration[index / 8] ^= 1 << (index % 8);
}
static void __dst_alloc_clr_parent_split__dmemtypedef_NODE__typedef_declaration(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemtypedef_NODE__typedef_declaration[index / 8] &= ~(1 << (index % 8));
}
struct __dst_alloc_list_base_t __dst_alloc_buckets__dmemtypedef_NODE__typedef_declaration[10 + 1] = {{1, 1}};
static __dst_alloc_bucket_size_t __dst_alloc_bucket_for_request__dmemtypedef_NODE__typedef_declaration(
        __dst_alloc_size_t request) {
    __dst_alloc_bucket_size_t bucket = 10;
    __dst_alloc_size_t size = 1;
    while (size < request) {
        size <<= 1;
        bucket -= 1;
    }
    return bucket;
}
static void __dst_alloc_list_push__dmemtypedef_NODE__typedef_declaration(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dst_alloc_buckets__dmemtypedef_NODE__typedef_declaration[bucket].prev;
    __dmemtypedef_NODE__typedef_declaration[entry]._link.prev = prev;
    __dmemtypedef_NODE__typedef_declaration[entry]._link.next = 0;
    if (prev == 0) __dst_alloc_buckets__dmemtypedef_NODE__typedef_declaration[bucket].next = entry;
    else __dmemtypedef_NODE__typedef_declaration[prev]._link.next = entry;
    __dst_alloc_buckets__dmemtypedef_NODE__typedef_declaration[bucket].prev = entry;
}
static void __dst_alloc_list_remove__dmemtypedef_NODE__typedef_declaration(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dmemtypedef_NODE__typedef_declaration[entry]._link.prev;
    __dst_alloc_size_t next = __dmemtypedef_NODE__typedef_declaration[entry]._link.next;
    if (prev == 0) __dst_alloc_buckets__dmemtypedef_NODE__typedef_declaration[bucket].next = next;
    else __dmemtypedef_NODE__typedef_declaration[prev]._link.next = next;
    if (next == 0) __dst_alloc_buckets__dmemtypedef_NODE__typedef_declaration[bucket].prev = prev;
    else __dmemtypedef_NODE__typedef_declaration[next]._link.prev = prev;
}
static __dst_alloc_size_t __dst_alloc_list_pop__dmemtypedef_NODE__typedef_declaration(
        __dst_alloc_bucket_size_t bucket) {
    __dst_alloc_size_t back = __dst_alloc_buckets__dmemtypedef_NODE__typedef_declaration[bucket].prev;
    if (back == 0) return 0;
    __dst_alloc_list_remove__dmemtypedef_NODE__typedef_declaration(bucket, back);
    return back;
}
static __dst_alloc_size_t __dst_alloc_index_for_node__dmemtypedef_NODE__typedef_declaration(
        __dst_alloc_size_t bucket_index,
        __dst_alloc_bucket_size_t bucket) {
    return 1 + ((bucket_index - (1 << bucket) + 1) << (10 - bucket));
}
static __dst_alloc_size_t __dst_alloc_node_for_index__dmemtypedef_NODE__typedef_declaration(
        __dst_alloc_size_t global_index,
        __dst_alloc_bucket_size_t bucket) {
    return ((global_index - 1) >> (10 - bucket)) + (1 << bucket) - 1;
}
void __dst_alloc_init__dmemtypedef_NODE__typedef_declaration() {
    for (int i = 0; i <= 10; i++) {
        __dst_alloc_buckets__dmemtypedef_NODE__typedef_declaration[i].prev = 0;
        __dst_alloc_buckets__dmemtypedef_NODE__typedef_declaration[i].next = 0;
    }
    __dst_alloc_list_push__dmemtypedef_NODE__typedef_declaration(0, 1);
}
__dst_alloc_size_t __dst_alloc_malloc__dmemtypedef_NODE__typedef_declaration(__dst_alloc_size_t request) {
    request = request / sizeof(::NODE);
    if (request > (1 << 10)) return 0;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemtypedef_NODE__typedef_declaration(request);
    __dst_alloc_bucket_size_t original_bucket = bucket;
    while ((__dst_alloc_bucket_size_t)(bucket + 1) != 0) {
        __dst_alloc_size_t ptr = __dst_alloc_list_pop__dmemtypedef_NODE__typedef_declaration(bucket);
        if (!ptr) {
            bucket--; continue;
        }
        __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemtypedef_NODE__typedef_declaration(ptr, bucket);
        if (i != 0) __dst_alloc_flip_parent_split__dmemtypedef_NODE__typedef_declaration(i);
        while (bucket < original_bucket) {
            i = ((i) * 2 + 1);
            __dst_alloc_clr_parent_split__dmemtypedef_NODE__typedef_declaration(i);
            __dst_alloc_flip_parent_split__dmemtypedef_NODE__typedef_declaration(i);
            bucket++;
            __dst_alloc_list_push__dmemtypedef_NODE__typedef_declaration(bucket, __dst_alloc_index_for_node__dmemtypedef_NODE__typedef_declaration(((i) + 1), bucket));
        }
        __dmemtypedef_NODE__typedef_declaration[ptr]._link.prev = request;
        return ptr;
    }
    return 0;
}
void __dst_alloc_free__dmemtypedef_NODE__typedef_declaration(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemtypedef_NODE__typedef_declaration(__dmemtypedef_NODE__typedef_declaration[ptr]._link.prev);
    __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemtypedef_NODE__typedef_declaration(ptr, bucket);
    while (i != 0) {
        __dst_alloc_flip_parent_split__dmemtypedef_NODE__typedef_declaration(i);
        if (__dst_alloc_test_parent_split__dmemtypedef_NODE__typedef_declaration(i)) break;
        __dst_alloc_list_remove__dmemtypedef_NODE__typedef_declaration(bucket, __dst_alloc_index_for_node__dmemtypedef_NODE__typedef_declaration(((((i) - 1) ^ 1) + 1), bucket));
        i = (((i) - 1) / 2);
        bucket--;
    }
    __dst_alloc_list_push__dmemtypedef_NODE__typedef_declaration(bucket, __dst_alloc_index_for_node__dmemtypedef_NODE__typedef_declaration(i, bucket));
}
unsigned long long __dst_alloc_malloc__dmemtypedef_NODE__typedef_declaration(unsigned long long );
void __dst_alloc_free__dmemtypedef_NODE__typedef_declaration(unsigned long long );

void init(__didxtypedef_NODE__typedef_declaration *head)
{
   *head = 0L;
}

int *output_list(__didxtypedef_NODE__typedef_declaration head,int *curr)
{
  __didxtypedef_NODE__typedef_declaration temp;
  for (temp = head; temp; temp = (&(__dmemtypedef_NODE__typedef_declaration + temp + 0U) -> _data) -> next) {
     *(curr++) = (&(__dmemtypedef_NODE__typedef_declaration + temp + 0U) -> _data) -> data . info;
  }
  return curr;
}

__didxtypedef_NODE__typedef_declaration add(__didxtypedef_NODE__typedef_declaration node,DATA data)
{
  __didxtypedef_NODE__typedef_declaration temp = (__didxtypedef_NODE__typedef_declaration )(__dst_alloc_malloc__dmemtypedef_NODE__typedef_declaration(sizeof(NODE )));
  if (temp == 0L) {
    g_fallback = true;
    return 0L;
  }
  (&(__dmemtypedef_NODE__typedef_declaration + temp + 0U) -> _data) -> data = data;
  (&(__dmemtypedef_NODE__typedef_declaration + temp + 0U) -> _data) -> next = node;
  node = temp;
  return node;
}

void add_at(__didxtypedef_NODE__typedef_declaration node,DATA data)
{
  __didxtypedef_NODE__typedef_declaration temp = (__didxtypedef_NODE__typedef_declaration )(__dst_alloc_malloc__dmemtypedef_NODE__typedef_declaration(sizeof(NODE )));
  if (temp == 0L) {
    g_fallback = true;
    return ;
  }
  (&(__dmemtypedef_NODE__typedef_declaration + temp + 0U) -> _data) -> data = data;
  (&(__dmemtypedef_NODE__typedef_declaration + temp + 0U) -> _data) -> next = (&(__dmemtypedef_NODE__typedef_declaration + node + 0U) -> _data) -> next;
  (&(__dmemtypedef_NODE__typedef_declaration + node + 0U) -> _data) -> next = temp;
}

void remove_node(__didxtypedef_NODE__typedef_declaration head)
{
  __didxtypedef_NODE__typedef_declaration temp = (&(__dmemtypedef_NODE__typedef_declaration + head + 0U) -> _data) -> next;
  (&(__dmemtypedef_NODE__typedef_declaration + head + 0U) -> _data) -> next = (&(__dmemtypedef_NODE__typedef_declaration + (&(__dmemtypedef_NODE__typedef_declaration + head + 0U) -> _data) -> next + 0U) -> _data) -> next;
  __dst_alloc_free__dmemtypedef_NODE__typedef_declaration(temp);
}

struct __rect_packed_type_LC9sHn4mA2TzR 
{
  __didxtypedef_NODE__typedef_declaration local0;
  __didxtypedef_NODE__typedef_declaration local1;
  __didxtypedef_NODE__typedef_declaration local2;
  unsigned int _location;
  __didxtypedef_NODE__typedef_declaration _return;
  __didxtypedef_NODE__typedef_declaration _ret0;
}
;

__didxtypedef_NODE__typedef_declaration reverse_rec(__didxtypedef_NODE__typedef_declaration ptr,__didxtypedef_NODE__typedef_declaration previous)
{
  volatile struct __rect_packed_type_LC9sHn4mA2TzR __rect_packed_var_LC9sHn4mA2TzR[1024U];
  volatile unsigned int __rect_packed_top_LC9sHn4mA2TzR = 0U;
  __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . _location = 1U;
  __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . local0 = ptr;
  __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . local1 = previous;
  ++__rect_packed_top_LC9sHn4mA2TzR;
  __rect_func_L0_LC9sHn4mA2TzR:
  if (0U == __rect_packed_top_LC9sHn4mA2TzR) 
    return __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . _return;
  --__rect_packed_top_LC9sHn4mA2TzR;
  if (1U == __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . _location) 
    goto __rect_func_L1_LC9sHn4mA2TzR;
  if (2U == __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . _location) 
    goto __rect_func_L2_LC9sHn4mA2TzR;
  __rect_func_L1_LC9sHn4mA2TzR:
  __didxtypedef_NODE__typedef_declaration temp;
  if ((&(__dmemtypedef_NODE__typedef_declaration + __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . local0 + 0U) -> _data) -> next == 0L) {
    (&(__dmemtypedef_NODE__typedef_declaration + __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . local0 + 0U) -> _data) -> next = __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . local1;
    __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . _return = __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . local0;
    goto __rect_func_L0_LC9sHn4mA2TzR;
  }
   else {
    if (__rect_packed_top_LC9sHn4mA2TzR + 1U == 1024U) {;
g_fallback = true;
      __didxtypedef_NODE__typedef_declaration __temp0__;
      return __temp0__;
    }
    __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . _location = 2U;
    __rect_packed_var_LC9sHn4mA2TzR[1 + __rect_packed_top_LC9sHn4mA2TzR] . local1 = __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . local0;
    __rect_packed_var_LC9sHn4mA2TzR[1 + __rect_packed_top_LC9sHn4mA2TzR] . local0 = (&(__dmemtypedef_NODE__typedef_declaration + __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . local0 + 0U) -> _data) -> next;
    ++__rect_packed_top_LC9sHn4mA2TzR;
    __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . _location = 1U;
    goto __rect_func_L1_LC9sHn4mA2TzR;
    __rect_func_L2_LC9sHn4mA2TzR:
    __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . _ret0 = (__rect_packed_var_LC9sHn4mA2TzR[1 + __rect_packed_top_LC9sHn4mA2TzR] . _return);
    __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . local2 = __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . _ret0;
    (&(__dmemtypedef_NODE__typedef_declaration + __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . local0 + 0U) -> _data) -> next = __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . local1;
    __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . _return = __rect_packed_var_LC9sHn4mA2TzR[0 + __rect_packed_top_LC9sHn4mA2TzR] . local2;
    goto __rect_func_L0_LC9sHn4mA2TzR;
  }
  goto __rect_func_L0_LC9sHn4mA2TzR;
}

__didxtypedef_NODE__typedef_declaration sort_list(__didxtypedef_NODE__typedef_declaration head)
{
  __didxtypedef_NODE__typedef_declaration tmpPtr = head;
  __didxtypedef_NODE__typedef_declaration tmpNxt = (&(__dmemtypedef_NODE__typedef_declaration + head + 0U) -> _data) -> next;
  DATA tmp;
  while(tmpNxt != 0L){
    while(tmpNxt != tmpPtr){
      if ((&(__dmemtypedef_NODE__typedef_declaration + tmpNxt + 0U) -> _data) -> data . info < (&(__dmemtypedef_NODE__typedef_declaration + tmpPtr + 0U) -> _data) -> data . info) {
        tmp = (&(__dmemtypedef_NODE__typedef_declaration + tmpPtr + 0U) -> _data) -> data;
        (&(__dmemtypedef_NODE__typedef_declaration + tmpPtr + 0U) -> _data) -> data = (&(__dmemtypedef_NODE__typedef_declaration + tmpNxt + 0U) -> _data) -> data;
        (&(__dmemtypedef_NODE__typedef_declaration + tmpNxt + 0U) -> _data) -> data = tmp;
      }
      tmpPtr = (&(__dmemtypedef_NODE__typedef_declaration + tmpPtr + 0U) -> _data) -> next;
    }
    tmpPtr = head;
    tmpNxt = (&(__dmemtypedef_NODE__typedef_declaration + tmpNxt + 0U) -> _data) -> next;
  }
  return tmpPtr;
}

void process_top(int n,int *input,int *output,int *fallback)
{
  
#pragma HLS INTERFACE ap_memory port=input depth=10
  
#pragma HLS INTERFACE ap_memory port=output depth=45
  
#pragma HLS INTERFACE ap_memory port=fallback depth=1
  __didxtypedef_NODE__typedef_declaration head;
  __didxtypedef_NODE__typedef_declaration node;
  DATA element;
  int *curr;
  init(&head);
  for (int i = 0; i < n; i++) {
    element . info = input[i];
    head = add(head,element);
    if (!head) {
      g_fallback = true;
      goto fail;
    }
  }
  curr = output;
  curr = output_list(head,curr);
   *(curr++) = - 1;
  node = (&(__dmemtypedef_NODE__typedef_declaration + (&(__dmemtypedef_NODE__typedef_declaration + (&(__dmemtypedef_NODE__typedef_declaration + head + 0U) -> _data) -> next + 0U) -> _data) -> next + 0U) -> _data) -> next;
  element . info = 2000;
  add_at(node,element);
  if (g_fallback) 
    goto fail;
  curr = output_list(head,curr);
   *(curr++) = - 1;
  node = (&(__dmemtypedef_NODE__typedef_declaration + (&(__dmemtypedef_NODE__typedef_declaration + head + 0U) -> _data) -> next + 0U) -> _data) -> next;
  remove_node(node);
  head = sort_list(head);
  curr = output_list(head,curr);
   *(curr++) = - 1;
  head = reverse_rec(head,0L);
  if (g_fallback) 
    goto fail;
  curr = output_list(head,curr);
   *(curr++) = - 1;
  fail:
   *fallback = g_fallback;
}
