
#include "header.hpp"
#include <iostream>

int main() {
  int n = 6;
  int res = process_top(n);
  	if (res == 13) {
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

