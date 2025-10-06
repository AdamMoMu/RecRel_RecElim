#include "header.hpp"


void quickSort(int * vec, int low, int high) {
    if (low < high) {
        int pivot = vec[high];
        int ind = low - 1;

        for (int i = low; i <= high - 1; i++) {

            if (vec[i] <= pivot) {
                ind++;
                int temp = vec[ind];
                vec[ind] = vec[i];
                vec[i] = temp;
            }
        }
        int temp = vec[ind + 1];
        vec[ind + 1] = vec[high];
        vec[high] = temp;

        pivot = ind + 1;

        quickSort(vec, low, pivot - 1);
        quickSort(vec, pivot + 1, high);
    }
}

int process_top(int n, int arr[16]) {
	#pragma HLS INTERFACE port=arr ap_memory depth = 16
	quickSort(arr, 0, n-1);
    return 0;
}