#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "ui.h"

#define MAX_NUM_TURNS 1000

int main() {
    srand(time(NULL));

    auto board = init_board();
    Tile inp;
    Selection select;
    while (checkmate(&board) == PLAYER_NONE) {
        print_board(board);
        printf("Choose a piece to move: ");
        while(true) {
            inp = user_input();
            if (inp.row == -1) {
                printf("Invalid input.\n");
                continue;
            }
            select = select_tile(&board, inp.row, inp.col);
            if (!select.board) {
                printf("Cannot choose that.\n");
                continue;
            }
            break;
        }
        print_selection(select);
        printf("Where to move the piece? ");
        while(true) {
            inp = user_input();
            if (inp.row == -1) {
                printf("Invalid input.\n");
                continue;
            }
            
            if (!is_move_possible(select, inp.row, inp.col)) {
                printf("cannot move there.\n");
                continue;
            }
            
            board = move_selected_piece(&select, inp.row, inp.col);
            break;
        }
    }
    
    return 0;
}