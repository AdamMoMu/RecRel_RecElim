#include "header.hpp"

/*
 * This program was taken from HLSRecurse and modified. 
 * D. B. Thomas, "Synthesisable recursion for C++ HLS tools," 
 * doi: 10.1109/ASAP.2016.7760777. 
 */

void heap_sum(int *f, int start, int end)
{
    if(end - start <= 1){
        return;
    }else{
        heap_sum(f, start, start + (end-start) / 2);
        heap_sum(f, start + (end-start) / 2, end);
        f[start] += f[start + (end-start) / 2];
    }
}


int process_top(int *arr, int n) 
{
    #pragma HLS INTERFACE ap_memory port=arr depth=16
    heap_sum(arr, 0, n);
    return 0;
}