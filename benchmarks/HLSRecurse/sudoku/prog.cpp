#include "header.hpp"
#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;

int isValid(int number, int *puzzle, int row, int column) {
    int i=0;
    int sectorRow = 3*(row/3);
    int sectorCol = 3*(column/3);
    int row1 = (row+2)%3;
    int row2 = (row+4)%3;
    int col1 = (column+2)%3;
    int col2 = (column+4)%3;

    /* Check for the value in the given row and column */
    for (i=0; i<9; i++) {
        if (puzzle[i*9+column] == number) return false;
        if (puzzle[row*9+i] == number) return false;
    }

    /* Check the remaining four spaces in this sector */
    if(puzzle[ (row1+sectorRow)*9+col1+sectorCol] == number) return false;
    if(puzzle[ (row2+sectorRow)*9+col1+sectorCol] == number) return false;
    if(puzzle[ (row1+sectorRow)*9+col2+sectorCol] == number) return false;
    if(puzzle[ (row2+sectorRow)*9+col2+sectorCol] == number) return false;
    return true;
}

bool sudoku_func(int *puzzle, int row, int column)
{
    int row=0;
    int column=0;
    int nextNum=0;
    bool tmp=false;

    return run_function_old<bool>(
        Sequence(
            //[&](){ printf("row=%d, col=%d, addr=%u\n", row, column, row*9+column); },
            If([&](){ return 9==row; },
                    Return([&](){ return true; })
            ),
            // Is this element already set?  If so, we don't want to
            // change it.  Recur immediately to the next cell.
            If([&](){ return puzzle[row*9+column]; },
                IfElse([&](){ return column==8; },
                    RecurseAndReturn([&](){ return make_hls_state_tuple(row+1, 0,       0); }),
                    RecurseAndReturn([&](){ return make_hls_state_tuple(row, column+1,  0); })
                )
            ),
            // Try all possible values for the number
            [&](){ nextNum=1; },
            While([&](){ return nextNum<10; },
                If([&](){ return isValid(nextNum, puzzle, row, column); },
                    // Temporarily bind this entry to nextNum
                    [&](){ puzzle[row*9+column] = nextNum; },
                    IfElse([&](){ return column == 8; },
                        RecurseWithResult(tmp, [&](){ return make_hls_state_tuple(row+1, 0,      0); }),
                        RecurseWithResult(tmp, [&](){ return make_hls_state_tuple(row, column+1, 0); })
                    ),
                    If([&](){ return tmp; },
                        Return([&](){ return tmp; })
                    ),
                    // We failed to find a valid value for this, undo
                    [&](){ puzzle[row*9+column] = 0; }
                ),
                [&](){ nextNum=nextNum+1; }
            ),
            // Have to backtrack
            Return([&](){ return false; })
       ),
       row, column, nextNum, tmp
   );
}


int process_top(int *puzzle, int row, int column) {
#pragma HLS INTERFACE ap_memory port = puzzle depth=81
    sudoku_func(puzzle, row, column);
    return 0;
}