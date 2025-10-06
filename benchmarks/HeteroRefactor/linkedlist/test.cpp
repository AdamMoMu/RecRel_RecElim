
#include "header.hpp"
#include <iostream>

int main(int argc, char** argv) {
    int n = 10;
    int input[10] = {5, 3, 4, 2, 4, 5, 8, 0, 2, 3};
    int results[4 * n + 5];
    for (int i = 0; i < 4 * n + 5; i++) {
        results[i] = 0;
    }
    int fallback = 0;
    process_top(n, input, results, &fallback);
    for (int i = 0; i < 4 * n + 5; i++) {
        if (results[i] == -1) printf("\n");
        else printf("%d ", results[i]);
    }
    return 0;
}
