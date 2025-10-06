#include "header.hpp"
#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;

void merge(int* arr, int left, 
                     int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temp std::vectors
    int L[50];
	int R[50];

    // Copy data to temp vectors L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    // Merge the temp vectors back 
    // into arr[left..right]
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], 
    // if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], 
    // if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// begin is for left index and end is right index
// of the sub-array of arr to be sorted
void mergeSort(int* arr, int left, int right) {
    int mid;
    run_function_old<void>(
        IfElse(
            [&](){ return left >= right; },
            Return(),
            Sequence(
                [&](){ mid = left + (right - left) / 2; },
                Recurse([&](){ return make_hls_state_tuple(arr, left, mid); }),
                Recurse([&](){ return make_hls_state_tuple(arr, mid + 1, right); }),
                [&](){ merge(arr, left, mid, right); }
            )
        ),
        arr, left, right, mid
    );
}


int process_top(int n, int* arr) 
{
    #pragma HLS INTERFACE ap_memory port=arr depth=16
    mergeSort(arr, 0, n - 1);
    return 0;
}