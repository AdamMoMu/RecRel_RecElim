
#include "header.hpp"
#include <iostream>


int main() {
	int arr[8];
	int n = 8;
	for (int i = 0; i < n; i++) {
		arr[i] = i + 1;
	}
	int res = process_top(arr, n);
		if (res == 36) {
			printf("Good\n");
			printf("Res: %d\n", res);
		}
		else {
			printf("Fail\n");
			printf("Res: %d\n", res);
			return 1;
		}
		return 0;
}

