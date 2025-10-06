
#include "header.hpp"
#include <iostream>

int main() {
 int n = 50;
 int m = 6;
 int res = process_top(n,m);
 	if (res == 13) {
 		printf("Good\n");
 		printf("Index: %d\n", res);
 	}
 	else {
 		printf("Fail\n");
 		printf("Index: %d\n", res);
 		return 1;
 	}
		return 0;
}
