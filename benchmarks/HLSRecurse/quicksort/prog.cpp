#include "header.hpp"

#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;

void quickSort(int vec[16], int low, int high) {
    int pivot;
    run_function_old<void>(
        If([&](){ return low < high; },
            Sequence(
                [&](){
                    int pi = vec[high];
                    int ind = low - 1;

                    for (int i = low; i <= high - 1; i++) {

                        if (vec[i] <= pi) {
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
                },
                Sequence(
                    Recurse([&](){ return make_hls_state_tuple(vec, low, pivot - 1, 0); }),
                    Recurse([&](){ return make_hls_state_tuple(vec, pivot + 1, high, 0); })
                )
            )
        ),
        vec, low, high, pivot
    );
}

int process_top(int n, int arr[16]) {
	#pragma HLS INTERFACE port=arr ap_memory depth = 16
	quickSort(arr, 0, n-1);
    return 0;
}