
#include "header.hpp"
#include <iostream>

int main(int argc, char** argv) {
    int n = 10;
    int input[10] = {5, 3, 4, 2, 4, 5, 8, 0, 2, 3};
    int results[10];
    for (int i = 0; i < n; i++) {
        results[i] = 0;
    }
    int fallback = 0;
    process_top(n, input, results);
    for (int i = 0; i < 10; i++) {
        printf("%d ", results[i]);
    }
	std::cout << "\n";
    return 0;
}