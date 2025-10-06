
#include "header.hpp"
#include <iostream>



int main() {
	int n = 8;
	int arr[8] = {0};
	for (int i = 0; i < n; i++) {
		arr[i] = n - i;
	}

	int f = 0;
   int right = process_top(n, arr);

	if (arr[0] == 1) {
		printf("Good\n");
	}
	else {
		printf("Fail\n");

		return 1;
	}

   for (int i = 0; i < n; i++) {
   		if (arr[i] != i+1) return 1;
   		printf("%d", arr[i]);
   	}

	printf("\n");
    return 0;
}

