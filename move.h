#include <stdio.h>
#include "board.h"

typedef struct selection_t {
    Board *board;
    Square pos;
    Square possible_moves[NUM_SQUARES];
    int num_possible_moves;
    pid threatened_pieces[NUM_PIECES_BLACK]; //assume black and white have the same num of pieces
    int num_threatened_pieces;
} Selection;

const Selection select_none = { .board = NULL };

Selection select_tile(Board *, int, int);
Board move_selected_piece(Selection, int, int);
bool is_move_possible(Selection, int, int);
Player checkmate(Board *);