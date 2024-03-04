#include "pieces.h"

#define MAX_NUM_PIECES 32

typedef struct board_t {
    Piece pieces[MAX_NUM_PIECES];
    int num_pieces;
    int selected_piece;
    Player check;
    Player checkmate;
} Board;

Board init_board();
Board scrambled_board();
int get_piece(Board, int, int);
int possible_moves(Cell *, Board, int);
Board move(Board, Piece, Cell);
bool allowed_to_move(Board, Piece, Cell);
Player checkmate(Board);
Board random_move_for_piece(Board, int);
Board random_move(Board, Player);