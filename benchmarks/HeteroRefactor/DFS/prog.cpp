
struct __dst_alloc_list_base_t 
{
  unsigned int prev;
  unsigned int next;
}
;
// type __didxi was int *
typedef unsigned long long __didxi;
// type __didxclass_node was struct ::node{int value;struct node *left;struct node *right;}*
typedef unsigned long long __didxclass_node;
// === BEGIN REC SUPPORT LIBRARY ===
#include <stdio.h>
#include <stdlib.h>
const char *__dst_filename = "";
unsigned long long __dst_file = 0;
// === END REC SUPPORT LIBRARY ===
// #include "header.hpp"
#include <stdio.h>
#include <stdlib.h>

struct __dst_alloc_list__dmemi 
{
  struct __dst_alloc_list_base_t _link;
  int _data;
}
;
struct __dst_alloc_list__dmemi __dmemi[1025U];
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
unsigned char __dst_alloc_node_split__dmemi[(1 << (10)) / 8];
static unsigned char __dst_alloc_test_parent_split__dmemi(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    return (__dst_alloc_node_split__dmemi[index / 8] >> (index % 8)) & 1;
}
static void __dst_alloc_flip_parent_split__dmemi(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemi[index / 8] ^= 1 << (index % 8);
}
static void __dst_alloc_clr_parent_split__dmemi(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemi[index / 8] &= ~(1 << (index % 8));
}
struct __dst_alloc_list_base_t __dst_alloc_buckets__dmemi[10 + 1] = {{1, 1}};
static __dst_alloc_bucket_size_t __dst_alloc_bucket_for_request__dmemi(
        __dst_alloc_size_t request) {
    __dst_alloc_bucket_size_t bucket = 10;
    __dst_alloc_size_t size = 1;
    while (size < request) {
        size <<= 1;
        bucket -= 1;
    }
    return bucket;
}
static void __dst_alloc_list_push__dmemi(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dst_alloc_buckets__dmemi[bucket].prev;
    __dmemi[entry]._link.prev = prev;
    __dmemi[entry]._link.next = 0;
    if (prev == 0) __dst_alloc_buckets__dmemi[bucket].next = entry;
    else __dmemi[prev]._link.next = entry;
    __dst_alloc_buckets__dmemi[bucket].prev = entry;
}
static void __dst_alloc_list_remove__dmemi(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dmemi[entry]._link.prev;
    __dst_alloc_size_t next = __dmemi[entry]._link.next;
    if (prev == 0) __dst_alloc_buckets__dmemi[bucket].next = next;
    else __dmemi[prev]._link.next = next;
    if (next == 0) __dst_alloc_buckets__dmemi[bucket].prev = prev;
    else __dmemi[next]._link.prev = prev;
}
static __dst_alloc_size_t __dst_alloc_list_pop__dmemi(
        __dst_alloc_bucket_size_t bucket) {
    __dst_alloc_size_t back = __dst_alloc_buckets__dmemi[bucket].prev;
    if (back == 0) return 0;
    __dst_alloc_list_remove__dmemi(bucket, back);
    return back;
}
static __dst_alloc_size_t __dst_alloc_index_for_node__dmemi(
        __dst_alloc_size_t bucket_index,
        __dst_alloc_bucket_size_t bucket) {
    return 1 + ((bucket_index - (1 << bucket) + 1) << (10 - bucket));
}
static __dst_alloc_size_t __dst_alloc_node_for_index__dmemi(
        __dst_alloc_size_t global_index,
        __dst_alloc_bucket_size_t bucket) {
    return ((global_index - 1) >> (10 - bucket)) + (1 << bucket) - 1;
}
void __dst_alloc_init__dmemi() {
    for (int i = 0; i <= 10; i++) {
        __dst_alloc_buckets__dmemi[i].prev = 0;
        __dst_alloc_buckets__dmemi[i].next = 0;
    }
    __dst_alloc_list_push__dmemi(0, 1);
}
__dst_alloc_size_t __dst_alloc_malloc__dmemi(__dst_alloc_size_t request) {
    request = request / sizeof(int);
    if (request > (1 << 10)) return 0;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemi(request);
    __dst_alloc_bucket_size_t original_bucket = bucket;
    while ((__dst_alloc_bucket_size_t)(bucket + 1) != 0) {
        __dst_alloc_size_t ptr = __dst_alloc_list_pop__dmemi(bucket);
        if (!ptr) {
            bucket--; continue;
        }
        __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemi(ptr, bucket);
        if (i != 0) __dst_alloc_flip_parent_split__dmemi(i);
        while (bucket < original_bucket) {
            i = ((i) * 2 + 1);
            __dst_alloc_clr_parent_split__dmemi(i);
            __dst_alloc_flip_parent_split__dmemi(i);
            bucket++;
            __dst_alloc_list_push__dmemi(bucket, __dst_alloc_index_for_node__dmemi(((i) + 1), bucket));
        }
        __dmemi[ptr]._link.prev = request;
        return ptr;
    }
    return 0;
}
void __dst_alloc_free__dmemi(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemi(__dmemi[ptr]._link.prev);
    __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemi(ptr, bucket);
    while (i != 0) {
        __dst_alloc_flip_parent_split__dmemi(i);
        if (__dst_alloc_test_parent_split__dmemi(i)) break;
        __dst_alloc_list_remove__dmemi(bucket, __dst_alloc_index_for_node__dmemi(((((i) - 1) ^ 1) + 1), bucket));
        i = (((i) - 1) / 2);
        bucket--;
    }
    __dst_alloc_list_push__dmemi(bucket, __dst_alloc_index_for_node__dmemi(i, bucket));
}
unsigned long long __dst_alloc_malloc__dmemi(unsigned long long );
void __dst_alloc_free__dmemi(unsigned long long );
using namespace std;

struct node 
{
  int value;
  __didxclass_node left;
  __didxclass_node right;
}
;

struct __dst_alloc_list__dmemclass_node 
{
  struct __dst_alloc_list_base_t _link;
  struct node _data;
}
;
struct __dst_alloc_list__dmemclass_node __dmemclass_node[1025U];
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
unsigned char __dst_alloc_node_split__dmemclass_node[(1 << (10)) / 8];
static unsigned char __dst_alloc_test_parent_split__dmemclass_node(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    return (__dst_alloc_node_split__dmemclass_node[index / 8] >> (index % 8)) & 1;
}
static void __dst_alloc_flip_parent_split__dmemclass_node(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemclass_node[index / 8] ^= 1 << (index % 8);
}
static void __dst_alloc_clr_parent_split__dmemclass_node(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemclass_node[index / 8] &= ~(1 << (index % 8));
}
struct __dst_alloc_list_base_t __dst_alloc_buckets__dmemclass_node[10 + 1] = {{1, 1}};
static __dst_alloc_bucket_size_t __dst_alloc_bucket_for_request__dmemclass_node(
        __dst_alloc_size_t request) {
    __dst_alloc_bucket_size_t bucket = 10;
    __dst_alloc_size_t size = 1;
    while (size < request) {
        size <<= 1;
        bucket -= 1;
    }
    return bucket;
}
static void __dst_alloc_list_push__dmemclass_node(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dst_alloc_buckets__dmemclass_node[bucket].prev;
    __dmemclass_node[entry]._link.prev = prev;
    __dmemclass_node[entry]._link.next = 0;
    if (prev == 0) __dst_alloc_buckets__dmemclass_node[bucket].next = entry;
    else __dmemclass_node[prev]._link.next = entry;
    __dst_alloc_buckets__dmemclass_node[bucket].prev = entry;
}
static void __dst_alloc_list_remove__dmemclass_node(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dmemclass_node[entry]._link.prev;
    __dst_alloc_size_t next = __dmemclass_node[entry]._link.next;
    if (prev == 0) __dst_alloc_buckets__dmemclass_node[bucket].next = next;
    else __dmemclass_node[prev]._link.next = next;
    if (next == 0) __dst_alloc_buckets__dmemclass_node[bucket].prev = prev;
    else __dmemclass_node[next]._link.prev = prev;
}
static __dst_alloc_size_t __dst_alloc_list_pop__dmemclass_node(
        __dst_alloc_bucket_size_t bucket) {
    __dst_alloc_size_t back = __dst_alloc_buckets__dmemclass_node[bucket].prev;
    if (back == 0) return 0;
    __dst_alloc_list_remove__dmemclass_node(bucket, back);
    return back;
}
static __dst_alloc_size_t __dst_alloc_index_for_node__dmemclass_node(
        __dst_alloc_size_t bucket_index,
        __dst_alloc_bucket_size_t bucket) {
    return 1 + ((bucket_index - (1 << bucket) + 1) << (10 - bucket));
}
static __dst_alloc_size_t __dst_alloc_node_for_index__dmemclass_node(
        __dst_alloc_size_t global_index,
        __dst_alloc_bucket_size_t bucket) {
    return ((global_index - 1) >> (10 - bucket)) + (1 << bucket) - 1;
}
void __dst_alloc_init__dmemclass_node() {
    for (int i = 0; i <= 10; i++) {
        __dst_alloc_buckets__dmemclass_node[i].prev = 0;
        __dst_alloc_buckets__dmemclass_node[i].next = 0;
    }
    __dst_alloc_list_push__dmemclass_node(0, 1);
}
__dst_alloc_size_t __dst_alloc_malloc__dmemclass_node(__dst_alloc_size_t request) {
    request = request / sizeof(struct ::node);
    if (request > (1 << 10)) return 0;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemclass_node(request);
    __dst_alloc_bucket_size_t original_bucket = bucket;
    while ((__dst_alloc_bucket_size_t)(bucket + 1) != 0) {
        __dst_alloc_size_t ptr = __dst_alloc_list_pop__dmemclass_node(bucket);
        if (!ptr) {
            bucket--; continue;
        }
        __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemclass_node(ptr, bucket);
        if (i != 0) __dst_alloc_flip_parent_split__dmemclass_node(i);
        while (bucket < original_bucket) {
            i = ((i) * 2 + 1);
            __dst_alloc_clr_parent_split__dmemclass_node(i);
            __dst_alloc_flip_parent_split__dmemclass_node(i);
            bucket++;
            __dst_alloc_list_push__dmemclass_node(bucket, __dst_alloc_index_for_node__dmemclass_node(((i) + 1), bucket));
        }
        __dmemclass_node[ptr]._link.prev = request;
        return ptr;
    }
    return 0;
}
void __dst_alloc_free__dmemclass_node(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemclass_node(__dmemclass_node[ptr]._link.prev);
    __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemclass_node(ptr, bucket);
    while (i != 0) {
        __dst_alloc_flip_parent_split__dmemclass_node(i);
        if (__dst_alloc_test_parent_split__dmemclass_node(i)) break;
        __dst_alloc_list_remove__dmemclass_node(bucket, __dst_alloc_index_for_node__dmemclass_node(((((i) - 1) ^ 1) + 1), bucket));
        i = (((i) - 1) / 2);
        bucket--;
    }
    __dst_alloc_list_push__dmemclass_node(bucket, __dst_alloc_index_for_node__dmemclass_node(i, bucket));
}
unsigned long long __dst_alloc_malloc__dmemclass_node(unsigned long long );
void __dst_alloc_free__dmemclass_node(unsigned long long );
struct node ;
__didxi queue;
int front = 0;
int rear = - 1;
// Recursive function for DFS traversal

struct __rect_packed_type_L80Qr6fOJkZuR 
{
  __didxclass_node local0;
  unsigned int _location;
}
;

void dfs(__didxclass_node root)
{
  volatile struct __rect_packed_type_L80Qr6fOJkZuR __rect_packed_var_L80Qr6fOJkZuR[1024U];
  volatile unsigned int __rect_packed_top_L80Qr6fOJkZuR = 0U;
  __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . _location = 1U;
  __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . local0 = root;
  ++__rect_packed_top_L80Qr6fOJkZuR;
  __rect_func_L0_L80Qr6fOJkZuR:
  if (0U == __rect_packed_top_L80Qr6fOJkZuR) 
    return ;
  --__rect_packed_top_L80Qr6fOJkZuR;
  if (1U == __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . _location) 
    goto __rect_func_L1_L80Qr6fOJkZuR;
  if (2U == __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . _location) 
    goto __rect_func_L2_L80Qr6fOJkZuR;
  if (3U == __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . _location) 
    goto __rect_func_L3_L80Qr6fOJkZuR;
  __rect_func_L1_L80Qr6fOJkZuR:
  if ((&(__dmemclass_node + __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . local0 + 0U) -> _data) -> left != 0L) {
    if (__rect_packed_top_L80Qr6fOJkZuR + 1U == 1024U) {;
      return ;
    }
    __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . _location = 2U;
    __rect_packed_var_L80Qr6fOJkZuR[1 + __rect_packed_top_L80Qr6fOJkZuR] . local0 = (&(__dmemclass_node + __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . local0 + 0U) -> _data) -> left;
    ++__rect_packed_top_L80Qr6fOJkZuR;
    __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . _location = 1U;
    goto __rect_func_L1_L80Qr6fOJkZuR;
    __rect_func_L2_L80Qr6fOJkZuR:
    0;
  }
  (&(__dmemi + queue + ++rear) -> _data)[0U] = (&(__dmemclass_node + __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . local0 + 0U) -> _data) -> value;
  if ((&(__dmemclass_node + __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . local0 + 0U) -> _data) -> right != 0L) {
    if (__rect_packed_top_L80Qr6fOJkZuR + 1U == 1024U) {;
      return ;
    }
    __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . _location = 3U;
    __rect_packed_var_L80Qr6fOJkZuR[1 + __rect_packed_top_L80Qr6fOJkZuR] . local0 = (&(__dmemclass_node + __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . local0 + 0U) -> _data) -> right;
    ++__rect_packed_top_L80Qr6fOJkZuR;
    __rect_packed_var_L80Qr6fOJkZuR[0 + __rect_packed_top_L80Qr6fOJkZuR] . _location = 1U;
    goto __rect_func_L1_L80Qr6fOJkZuR;
    __rect_func_L3_L80Qr6fOJkZuR:
    0;
  }
  goto __rect_func_L0_L80Qr6fOJkZuR;
}

struct __rect_packed_type_LBqt89rHrNqbR 
{
  __didxclass_node local0;
  __didxclass_node local1;
  unsigned int _location;
}
;

void insert(__didxclass_node newitem,__didxclass_node root)
{
  volatile struct __rect_packed_type_LBqt89rHrNqbR __rect_packed_var_LBqt89rHrNqbR[1024U];
  volatile unsigned int __rect_packed_top_LBqt89rHrNqbR = 0U;
  __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . _location = 1U;
  __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local0 = newitem;
  __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local1 = root;
  ++__rect_packed_top_LBqt89rHrNqbR;
  __rect_func_L0_LBqt89rHrNqbR:
  if (0U == __rect_packed_top_LBqt89rHrNqbR) 
    return ;
  --__rect_packed_top_LBqt89rHrNqbR;
  if (1U == __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . _location) 
    goto __rect_func_L1_LBqt89rHrNqbR;
  if (2U == __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . _location) 
    goto __rect_func_L2_LBqt89rHrNqbR;
  if (3U == __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . _location) 
    goto __rect_func_L3_LBqt89rHrNqbR;
  __rect_func_L1_LBqt89rHrNqbR:
  if ((&(__dmemclass_node + __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local0 + 0U) -> _data) -> value > (&(__dmemclass_node + __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local1 + 0U) -> _data) -> value) {
    if ((&(__dmemclass_node + __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local1 + 0U) -> _data) -> right == 0L) 
      (&(__dmemclass_node + __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local1 + 0U) -> _data) -> right = __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local0;
     else {
      if (__rect_packed_top_LBqt89rHrNqbR + 1U == 1024U) {;
        return ;
      }
      __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . _location = 2U;
      __rect_packed_var_LBqt89rHrNqbR[1 + __rect_packed_top_LBqt89rHrNqbR] . local1 = (&(__dmemclass_node + __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local1 + 0U) -> _data) -> right;
      __rect_packed_var_LBqt89rHrNqbR[1 + __rect_packed_top_LBqt89rHrNqbR] . local0 = __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local0;
      ++__rect_packed_top_LBqt89rHrNqbR;
      __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . _location = 1U;
      goto __rect_func_L1_LBqt89rHrNqbR;
      __rect_func_L2_LBqt89rHrNqbR:
      0;
    }
// newitem->value <= root->value
  }
   else {
    if ((&(__dmemclass_node + __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local1 + 0U) -> _data) -> left == 0L) 
      (&(__dmemclass_node + __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local1 + 0U) -> _data) -> left = __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local0;
     else {
      if (__rect_packed_top_LBqt89rHrNqbR + 1U == 1024U) {;
        return ;
      }
      __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . _location = 3U;
      __rect_packed_var_LBqt89rHrNqbR[1 + __rect_packed_top_LBqt89rHrNqbR] . local1 = (&(__dmemclass_node + __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local1 + 0U) -> _data) -> left;
      __rect_packed_var_LBqt89rHrNqbR[1 + __rect_packed_top_LBqt89rHrNqbR] . local0 = __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . local0;
      ++__rect_packed_top_LBqt89rHrNqbR;
      __rect_packed_var_LBqt89rHrNqbR[0 + __rect_packed_top_LBqt89rHrNqbR] . _location = 1U;
      goto __rect_func_L1_LBqt89rHrNqbR;
      __rect_func_L3_LBqt89rHrNqbR:
      0;
    }
  }
  goto __rect_func_L0_LBqt89rHrNqbR;
}

int process_top(int n,int *a,int *out)
{
  
#pragma HLS INTERFACE ap_memory port=a depth=10
  
#pragma HLS INTERFACE ap_memory port=out depth=10
  __didxclass_node root = 0L;
  __didxclass_node newitem = 0L;
  for (int i = 0; i < n; i++) {
    newitem = ((__didxclass_node )(__dst_alloc_malloc__dmemclass_node(sizeof(struct node ))));
    (&(__dmemclass_node + newitem + 0U) -> _data) -> left = (&(__dmemclass_node + newitem + 0U) -> _data) -> right = 0L;
    (&(__dmemclass_node + newitem + 0U) -> _data) -> value = a[i];
    if (root == 0L) {
      root = newitem;
    }
     else {
      insert(newitem,root);
    }
  }
  queue = ((__didxi )(__dst_alloc_malloc__dmemi(sizeof(int ) * n)));
  dfs(root);
  for (int i = 0; i < n; i++) 
    out[i] = (&(__dmemi + queue + i) -> _data)[0U];
  return 0;
}
