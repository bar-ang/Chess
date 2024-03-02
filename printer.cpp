#include <stdio.h>
#include "printer.h"

bool sign(char *str, PieceType type) {
    switch (type) {
        case PIECE_KING:
            sprintf(str, "K");
            break;
        case PIECE_QUEEN:
            sprintf(str, "Q");
            break;
        case PIECE_KNIGHT:
            sprintf(str, "N");
            break;
        case PIECE_BISHOP:
            sprintf(str, "B");
            break;
        case PIECE_ROOK:
            sprintf(str, "R");
            break;
        case PIECE_PAWN:
            sprintf(str, "P");
            break;
        default:
            return false;
    }
    return true;
}

void print_board(Board board) {
    bool found;
    char str[3];
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            found = false;
            for (Piece *p = board.pieces; p < board.pieces + board.num_pieces; p++) {
                if (p->row == i && p->col == j) {
                    found = sign(str, p->type) || found;
                    printf("%s ", str);
                }
            }
            if (!found) printf("%c ", EMPTY);
        }
        printf("\n");
    }
}