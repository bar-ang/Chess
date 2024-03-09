#define NUM_ROWS 8
#define NUM_COLS 8
#define NUM_SQUARES (NUM_ROWS * NUM_COLS)

#define IN_BOUNDS(c) ((c).row >= 0 && (c).row < NUM_ROWS && (c).col >= 0 && (c).col < NUM_COLS)
#define OPPONENT_OF(p) ((p) == PLAYER_WHITE ? PLAYER_BLACK : PLAYER_WHITE)

#define NO_SQUARE { .row = -1, .col = -1 }

typedef enum player_enum {
    PLAYER_NONE, PLAYER_BLACK, PLAYER_WHITE
} Player;

typedef enum piece_type_enum {
    PIECE_NONE, PIECE_KING, PIECE_QUEEN, PIECE_KNIGHT, PIECE_BISHOP, PIECE_ROOK, PIECE_PAWN
} PieceType;

typedef struct square_t {
    int row;
    int col;
} Square;

typedef struct piece_t {
    PieceType type;
    Player player;
} Piece;