#include <time.h>
#include <stdlib.h>
#include "ui.h"

#define MAX_NUM_TURNS 1000

#define STARTING_PLAYER PLAYER_WHITE
#define WHITE_IS_HUMAN true
#define BLACK_IS_HUMAN false

Board play_turn_computer(Board board, Player player) {
    return random_move(board, player);
}

Board play_turn_human(Board board, Player player) {
    Tile inp;
    Selection select;
    printf("Choose a piece to move: ");
    while(true) {
        inp = user_input();
        if (inp.row == -1) {
            printf("Invalid input.\n");
            continue;
        }

        auto pid = BOARD2(board, inp.row, inp.col);
        if (pid == NO_PIECE) {
            printf("No piece on this tile\n");
            continue;
        }

        if (board.pieces[pid].player != player) {
            printf("That's your opponent's piece.\n");
            continue;
        }
        
        select = select_tile(&board, inp.row, inp.col);
        if (!select.board) {
            printf("Cannot choose that.\n");
            continue;
        }
        
        if (select.num_possible_moves == 0) {
            printf("This piece cannot move.\n");
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

        return move_selected_piece(&select, inp.row, inp.col);
    }
}

int main() {
    srand(time(NULL));

    int round = 0;
    auto board = init_board();
    Tile inp;
    Selection select;

    Player turn;
    while (checkmate(&board) == PLAYER_NONE) {
        turn = round % 2 == 0 ? STARTING_PLAYER : OPPONENT_OF(STARTING_PLAYER);
        print_board(board);
        if ((turn == PLAYER_WHITE && WHITE_IS_HUMAN) || (turn == PLAYER_BLACK && BLACK_IS_HUMAN))
            board = play_turn_human(board, turn);
        else
            board = play_turn_computer(board, turn);
        round++;
    }
    
    return 0;
}