#include "pieces.h"

#define NUM_PIECES_WHITE 16
#define NUM_PIECES_BLACK 16
#define NUM_PIECES (NUM_PIECES_WHITE + NUM_PIECES_BLACK)

#define BOARD(b, x, y) ((b).board[(x)*(NUM_ROWS)+(y)])
#define BOARD2(b, s) ((b).board[((s).row)*(NUM_ROWS)+((s).col)])

#define NO_PIECE -1

typedef int pid;

typedef struct board_t {
    pid board[NUM_ROWS*NUM_COLS];
    Piece pieces[NUM_PIECES];
    int num_times_piece_has_moved[NUM_PIECES];
} Board;

Board init_empty_board();
Board init_board();
Board set_piece(Board, pid, PieceType, Player, int, int);
Board scrambled_board();
int get_piece(Board, int, int);
int possible_moves(Tile *, Board, int);
Board move(Board, Piece, Tile);
bool allowed_to_move(Board, Piece, Tile);
Tile find_king(Board *, Player);
bool compare_boards(Board *, Board *);
int board_score(Board *);
int player_score(Board *, Player);