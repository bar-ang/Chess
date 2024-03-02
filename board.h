#include "pieces.h"

#define MAX_NUM_PIECES 32
#define NUM_ROWS 8
#define NUM_COLS 8

typedef struct board_t {
    Piece pieces[MAX_NUM_PIECES];
    int num_pieces;
    int selected_piece;
} Board;

Board init_board();
Board scrambled_board();