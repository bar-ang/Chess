#include <stdio.h>
#include "board.h"

#define SELECTED_PIECE(sel) ((sel)->board->pieces[BOARD2(*((sel)->board), (sel)->pos)])

typedef struct selection_t {
    Board *board;
    Tile pos;
Tile possible_moves[NUM_TILES];
    int possible_moves_len;
    pid threatened_pieces[NUM_PIECES_BLACK]; //assume black and white have the same num of pieces
    int num_threatened_pieces;
} Selection;

const Selection select_none = { .board = NULL };

Selection select_tile(Board *, int, int);
Board move_selected_piece(Selection *, int, int, pid *);
bool is_move_possible(Selection *, int, int);
Player checkmate(Board *);
bool stalemate(Board *);
Selection unselect(Board *);
Board random_move_for_piece(Board, int);
Board random_move(Board, Player, Tile *, Tile *);
int adjacent_boards(Board *, Player, int, int, Board *);
int num_possible_moves(Selection *);