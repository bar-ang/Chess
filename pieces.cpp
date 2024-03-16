#include "pieces.h"

int piece_score(Piece piece) {
    switch (piece.type) {
        case PIECE_KING:
            return 0;
        case PIECE_QUEEN:
            return 8;
        case PIECE_ROOK:
            return 5;
        case PIECE_BISHOP:
        case PIECE_KNIGHT:
            return 3;
        case PIECE_PAWN:
            return 1;
        default:
            return -9999;
    }
}