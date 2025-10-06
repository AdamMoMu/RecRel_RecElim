
#include "header.hpp"
#include <iostream>


int main() {
 int n = 3;
 int m = 4;
 int res = process_top(n,m);
 	if (res == 125) {
 		printf("Good\n");
 		printf("Result: %d\n", res);
 	}
 	else {
 		printf("Fail\n");
 		printf("Result: %d\n", res);
 		return 1;
 	}
		return 0;
}
