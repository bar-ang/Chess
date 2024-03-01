typedef enum player_enum {
    PLAYER_NONE, PLAYER_BLACK, PLAYER_WHITE
} Player;

typedef enum piece_type_enum {
    PIECE_NONE, PIECE_KING, PIECE_QUEEN, PIECE_KNIGHT, PIECE_BISHOP, PIECE_ROOK, PIECE_PAWN
} PieceType;

typedef struct piece_t {
    PieceType type;
    Player player;
    int row;
    int col;
} Piece;