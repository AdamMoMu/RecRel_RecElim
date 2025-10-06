
#include "header.hpp"
#include <iostream>

using namespace std;
int main() {
    int puzzle[81] = {
        0,0,0, 0,6,3, 8,0,0,
        4,0,6, 5,0,0, 0,0,0,
        5,0,3, 0,9,0, 0,2,0,

        3,1,0, 0,0,5, 0,0,2,
        0,0,4, 1,2,7, 3,0,0,
        9,2,0, 4,0,0, 0,7,1,

        0,0,0, 0,0,0, 2,8,3,
        0,6,2, 0,0,0, 7,4,0,
        7,0,0, 0,0,0, 0,0,0};



    process_top(puzzle, 0, 0);
    
    for (int i = 0; i < 81; i++) {
        if (i % 9 == 0) cout << endl;
        cout << puzzle[i] << " ";
    }
    cout << endl;
    return 0;
}
