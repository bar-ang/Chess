#include "pieces.h"

#define MAX_NUM_PIECES 32

typedef struct board_t {
    Piece pieces[MAX_NUM_PIECES];
    int num_pieces;
} Board;

Board init_board();