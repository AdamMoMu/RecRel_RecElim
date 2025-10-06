
#include "header.hpp"
#include <iostream>


int main() {
	int n = 1024;
	int arr[1024] = {0};
	for (int i = 0; i < n; i++) {
		arr[i] = i;
	}
	int f = 0;
	int index = process_top(1024, 0, arr);

	if (index == 0) {
		printf("Good\n");
		printf("Index: %d\n", index);
	}
	else {
		printf("Fail\n");
		printf("Index: %d\n", index);

		return 1;
	}

	return 0;
}
