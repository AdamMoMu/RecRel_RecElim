#include "header.hpp"
#include <stdio.h>
#include <stdlib.h>

#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;


struct node {
  int value;
  struct node *left, *right;
};

int *queue;

void dfs(node *root) {
  run_function_old<void>(
      Sequence(
          If([&](){ return root->left != NULL; },
              Recurse([&](){ return make_hls_state_tuple(root->left); })
          ),
          [&](){ std::cout << root->value << " "; },
          If([&](){ return root->right != NULL; },
              Recurse([&](){ return make_hls_state_tuple(root->right); })
          )
      ),
      root
  );
}


void insert(node *newitem, node *root) {
  run_function_old<void>(
      IfElse([&](){ return newitem->value > root->value; },
          Sequence(
              IfElse([&](){ return root->right == NULL; },
                  Sequence(
                      [&](){ root->right = newitem; },
                      Return()
                  ),
                  Recurse([&](){ return make_hls_state_tuple(newitem, root->right); })
              )
          ),
          Sequence(
              IfElse([&](){ return root->left == NULL; },
                  Sequence(
                      [&](){ root->left = newitem; },
                      Return()
                  ),
                  Recurse([&](){ return make_hls_state_tuple(newitem, root->left); })
              )
          )
      ),
      newitem, root
  );
}
  
  
  int process_top(int n, int* a, int *out) {
    #pragma HLS INTERFACE ap_memory port=a depth=10
    #pragma HLS INTERFACE ap_memory port=out depth=10
    node *root = NULL;
    node *newitem = NULL ;
  
    for (int i = 0; i < n; i++) {
        
        newitem = (node*)malloc(sizeof(node));
  
        newitem->left = newitem->right = NULL;
        newitem->value = a[i];
        
        if (root == NULL) {
            root = newitem;
        } else {
            insert(newitem, root);
        }
    }

    queue = (int *)malloc(sizeof(int) * n);
  
    dfs(root);
    for (int i = 0; i < n; i++)
        out[i] = queue[i];

    delete queue;

    return 0;
  }