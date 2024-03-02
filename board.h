#include "pieces.h"

#define MAX_NUM_PIECES 32

typedef struct board_t {
    Piece pieces[MAX_NUM_PIECES];
    int num_pieces;
    int selected_piece;
} Board;

Board init_board();
Board scrambled_board();
int get_piece(Board, int, int);
int possible_moves(Cell *, Board, int);