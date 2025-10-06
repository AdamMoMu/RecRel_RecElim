#include "header.hpp"

void merge3(int *arr, int left, int mid1, int mid2, int right) {
    int n1 = mid1 - left + 1;
    int n2 = mid2 - mid1;
    int n3 = right - mid2;

    // temporary buffers (size 512 assumes max segment ≤512)
    int A[512], B[512], C[512];

    for (int i = 0; i < n1; i++)       A[i] = arr[left + i];
    for (int i = 0; i < n2; i++)       B[i] = arr[mid1 + 1 + i];
    for (int i = 0; i < n3; i++)       C[i] = arr[mid2 + 1 + i];

    int i = 0, j = 0, k = 0;
    int idx = left;

    // 1) while all three have elements, take the smallest of A[i], B[j], C[k]
    while (i < n1 && j < n2 && k < n3) {
        if (A[i] <= B[j] && A[i] <= C[k]) {
            arr[idx++] = A[i++];
        }
        else if (B[j] <= A[i] && B[j] <= C[k]) {
            arr[idx++] = B[j++];
        }
        else {
            arr[idx++] = C[k++];
        }
    }

    // 2) now merge the remaining two arrays
    // 2a) A & B
    while (i < n1 && j < n2) {
        arr[idx++] = (A[i] <= B[j] ? A[i++] : B[j++]);
    }
    // 2b) B & C
    while (j < n2 && k < n3) {
        arr[idx++] = (B[j] <= C[k] ? B[j++] : C[k++]);
    }
    // 2c) A & C
    while (i < n1 && k < n3) {
        arr[idx++] = (A[i] <= C[k] ? A[i++] : C[k++]);
    }

    // 3) copy any leftovers
    while (i < n1) arr[idx++] = A[i++];
    while (j < n2) arr[idx++] = B[j++];
    while (k < n3) arr[idx++] = C[k++];
}

// 3-way mergesort: split into [L..mid1], [mid1+1..mid2], [mid2+1..R]
void mergeSort3(int *arr, int left, int right) {
    if (left >= right) return;

    // two split-points
    int third = (right - left) / 3;
    int mid1  = left      + third;
    int mid2  = left + 2*third;

    // sort the three chunks
    mergeSort3(arr, left,      mid1);
    mergeSort3(arr, mid1 + 1,  mid2);
    mergeSort3(arr, mid2 + 1,  right);

    // merge all three back together
    merge3(arr, left, mid1, mid2, right);
}


int process_top(int n, int* arr) 
{
    #pragma HLS INTERFACE ap_memory port=arr depth=16
    mergeSort3(arr, 0, n - 1);
    return 0;
}