#include "pieces.h"

#define MAX_NUM_PIECES 32

typedef struct board_t {
    PieceType pieces[MAX_NUM_PIECES];
} Board;

Board init_board();