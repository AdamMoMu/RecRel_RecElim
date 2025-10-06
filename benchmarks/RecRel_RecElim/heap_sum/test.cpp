
#include "header.hpp"
#include <iostream>


int main() {
	int n = 16;
	int arr[16] = {0};
	for (int i = 0; i < n; i++) {
		arr[i] = i + 1;
	}
	
	int index = process_top(arr, 16);

	if (arr[0] == 136) {
		printf("Good\n");
		printf("Index: %d\n", arr[0]);
	}
	else {
		printf("Fail\n");
		printf("Index: %d\n", arr[0]);

		return 1;
	}

	return 0;
}
