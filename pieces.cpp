#include "pieces.h"

int move_map(Cell *map, Piece p) {
    int count = 0;
    int dir;
    Cell c;
    switch (p.type) {
        case PIECE_KING:
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    if (p.loc.row + i < 0 || p.loc.row + i >= NUM_ROWS) continue;
                    if (p.loc.col + j < 0 || p.loc.col + j >= NUM_COLS) continue;
                    c.row = p.loc.row + i;
                    c.col = p.loc.col + j;
                    map[count++] = c;
                }
            break;
        case PIECE_BISHOP:
            for (int i = -8; i <= 8; i++) {
                c.row = p.loc.row + i;
                c.col = p.loc.col + i;

                if (IN_BOUNDS(c))
                    map[count++] = c;

                c.row = p.loc.row + i;
                c.col = p.loc.col - i;

                if (IN_BOUNDS(c))
                    map[count++] = c;
            }
            break;
        case PIECE_KNIGHT:
            c.row = p.loc.row + 1;
            c.col = p.loc.col + 2;
            if (IN_BOUNDS(c)) map[count++] = c;

            c.row = p.loc.row - 1;
            c.col = p.loc.col + 2;
            if (IN_BOUNDS(c)) map[count++] = c;

            c.row = p.loc.row + 1;
            c.col = p.loc.col - 2;
            if (IN_BOUNDS(c)) map[count++] = c;

            c.row = p.loc.row - 1;
            c.col = p.loc.col - 2;
            if (IN_BOUNDS(c)) map[count++] = c;

            c.row = p.loc.row + 2;
            c.col = p.loc.col + 1;
            if (IN_BOUNDS(c)) map[count++] = c;

            c.row = p.loc.row - 2;
            c.col = p.loc.col + 1;
            if (IN_BOUNDS(c)) map[count++] = c;

            c.row = p.loc.row + 2;
            c.col = p.loc.col - 1;
            if (IN_BOUNDS(c)) map[count++] = c;

            c.row = p.loc.row - 2;
            c.col = p.loc.col - 1;
            if (IN_BOUNDS(c)) map[count++] = c;
            break;
        case PIECE_QUEEN:
            for (int i = -8; i <= 8; i++) {
                c.row = i;
                c.col = p.loc.col;
                if (IN_BOUNDS(c))
                    map[count++] = c;

                c.row = p.loc.row;
                c.col = i;
                if (IN_BOUNDS(c))
                    map[count++] = c;
                
                c.row = p.loc.row + i;
                c.col = p.loc.col + i;

                if (IN_BOUNDS(c))
                    map[count++] = c;

                c.row = p.loc.row + i;
                c.col = p.loc.col - i;
                
                if (IN_BOUNDS(c))
                    map[count++] = c;
            }
            break;
        case PIECE_ROOK:
            for (int i = 0; i < 8; i++) {
                c.row = i;
                c.col = p.loc.col;
                if (IN_BOUNDS(c))
                    map[count++] = c;

                c.row = p.loc.row;
                c.col = i;
                if (IN_BOUNDS(c))
                    map[count++] = c;
            }
            break;
        case PIECE_PAWN:
            dir = (p.player == PLAYER_WHITE) ? 1 : -1;
            c.row = p.loc.row + dir;
            c.col = p.loc.col;
            if (IN_BOUNDS(c)) map[count++] = c;

            if (p.loc.row == (p.player == PLAYER_WHITE ? 0 : 6)) {
                c.row = p.loc.row + 2*dir;
                c.col = p.loc.col;
                if (IN_BOUNDS(c)) map[count++] = c;
            }
            break;

        default:
            break;
    }

    return count;
}