#define NUM_ROWS 8
#define NUM_COLS 8
#define NUM_CELLS (NUM_ROWS * NUM_COLS)

#define IN_BOUNDS(c) ((c).row >= 0 && (c).row < NUM_ROWS && (c).col >= 0 && (c).col < NUM_COLS)

typedef enum player_enum {
    PLAYER_NONE, PLAYER_BLACK, PLAYER_WHITE
} Player;

typedef enum piece_type_enum {
    PIECE_NONE, PIECE_KING, PIECE_QUEEN, PIECE_KNIGHT, PIECE_BISHOP, PIECE_ROOK, PIECE_PAWN
} PieceType;

typedef struct cell_t {
    int row;
    int col;
} Cell;

typedef struct piece_t {
    PieceType type;
    Player player;
    Cell loc;
} Piece;

int move_map(Cell *map, Piece p);