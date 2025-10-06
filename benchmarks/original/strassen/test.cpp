
#include "header.hpp"
#include <iostream>
#include <vector>

using namespace std;
int main() {
    int n,l,m;
    n = 4;
    l = 4;
    m = 4;
    int matrix_A[16] = { 1, 3, 1, 4, 9, 2, 7, 5 , 3, 8, 3, 10, 1, 2, 5, 1 };
    int matrix_B[16] = { 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 2, 2, 2, 2 };

    int result_matrix[16];           
    int fallback = 0;               
    process_top(&n, &l, &m, matrix_A, matrix_B, result_matrix, &fallback);
    for (int i = 0; i <= 3; i++) {
        for (int j = 0; j <= 3; j++) {
            cout << result_matrix[i * 4 +  j] << " ";
        }
        cout << endl;
    }
    return 0;
}