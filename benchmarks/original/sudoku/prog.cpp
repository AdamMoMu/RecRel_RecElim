#include "header.hpp"


/*
 * This program was taken from HLSRecurse. 
 * D. B. Thomas, "Synthesisable recursion for C++ HLS tools," 
 * doi: 10.1109/ASAP.2016.7760777. 
 */

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
    if( 9==row ) {
        return true;
    }
    // Is this element already set?  If so, we don't want to
    // change it.  Recur immediately to the next cell.
    if( puzzle[row*9+column] ){
        if( column==8 ){
            return sudoku_func(puzzle, row+1, 0);
        }else{
            return sudoku_func(puzzle, row, column+1);
        }
    }
    for(int nextNum=1; nextNum<10; nextNum++)
    {
        if(isValid(nextNum, puzzle, row, column)){
            // Temporarily bind this entry to nextNum
            puzzle[row*9+column] = nextNum;
            if(column == 8){
                if(sudoku_func(puzzle, row+1, 0))
                    return true;
            }else{
                if(sudoku_func(puzzle, row, column+1))
                    return true;
            }
            // We failed to find a valid value for this, undo
            puzzle[row*9+column] = 0;
        }
    }
    // Have to backtrack
    return false;
}


int process_top(int *puzzle, int row, int column) {
#pragma HLS INTERFACE ap_memory port = puzzle depth=81
    sudoku_func(puzzle, row, column);
    return 0;
}