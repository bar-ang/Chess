#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "printer.h"

#define MAX_NUM_TURNS 1000

int main() {
    srand(time(NULL));

    Board board = init_empty_board();
    board = set_piece(board, 0, PIECE_PAWN, PLAYER_BLACK, 6, 3);
    board = set_piece(board, 1, PIECE_KING, PLAYER_BLACK, 2, 2);
    board = set_piece(board, 2, PIECE_QUEEN, PLAYER_BLACK, 2, 4);
    //board = set_piece(board, 1, PIECE_KING, PLAYER_BLACK, 6, 3);
    auto sel = select_tile(&board, 6, 3);
    print_board(board);
    printf("\n");
    printf("\n");
    print_selection(sel);
    /*
    int turn = 0;

    print_board(board);
    for (turn = 0; checkmate(board) == PLAYER_NONE && turn < MAX_NUM_TURNS; turn++)
        board = random_move(board, turn % 2 == 0 ? PLAYER_WHITE : PLAYER_BLACK);

    printf("\n");
    print_board(board);

    switch(checkmate(board)) {
        case PLAYER_BLACK: {
            printf("WHITE wins!\n");
            break;
        }
        case PLAYER_WHITE: {
            printf("BLACK wins!\n");
            break;
        }
        default: {
            printf("Reached %d turns with no winner.\n", turn);
            break;
        }
    }

    printf("game ended after %d turns.\n", turn);
    */
    
    return 0;
}