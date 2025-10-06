#include "header.hpp"
#include <stdio.h>
#include <stdlib.h>

/*
 * This program was taken from HeteroRefactor. 
 * Jason Lau, Aishwarya Sivaraman, Qian Zhang, Muhammad Ali Gulzar, 
 * Jason Cong, and Miryung Kim. 2020. HeteroRefactor: refactoring 
 * for heterogeneous computing with FPGA. https://doi.org/10.1145/3377811.3380340
 */

using namespace std;

struct node {
    int value;
    struct node *left, *right;
  };
  typedef struct node;
  

  int *queue;
  int front = 0, rear = -1;

  // Recursive function for DFS traversal
  void dfs(node *root) {
    if (root->left != NULL) {
      dfs(root->left);
    }

    queue[++rear] = root->value;
  
    if (root->right != NULL) {
      dfs(root->right);
    }
  }
  
  void insert(node *newitem, node *root) {
      if (newitem->value > root->value) {
          if (root->right == NULL)
              root->right = newitem;
          else
              insert(newitem, root->right);
      } else { // newitem->value <= root->value
          if (root->left  ==  NULL)
              root->left = newitem;
          else
              insert(newitem, root->left);
      }
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