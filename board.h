#include "pieces.h"

#define MAX_NUM_PIECES 32

typedef struct board_t {
    piece_type pieces[MAX_NUM_PIECES];
} board;