#include "move.h"

bool select_update(Selection *sel, Board *board, Square c, Piece piece) {
    auto pid = BOARD2(*board, c);
    if (pid == NO_PIECE)
        sel->possible_moves[sel->num_possible_moves++] = c;
    else {
        auto p2 = board->pieces[pid];
        if (piece.player != p2.player) {
            sel->possible_moves[sel->num_possible_moves++] = c;
            sel->threatened_pieces[sel->num_threatened_pieces++] = pid;
        }
        return true;
    }
    return false;
}

Selection select_tile(Board *board, int row, int col) {
    Selection sel;
    
    sel.board = board;
    sel.pos.row = row;
    sel.pos.col = col;
    sel.num_possible_moves = 0;
    sel.num_threatened_pieces = 0;

    auto pid = BOARD(*board, row, col);
    if (pid == NO_PIECE)
        return select_none;
    auto piece = board->pieces[pid];

    switch (piece.type) {
        case PIECE_KING: {
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    Square c = {.row = row + i, .col = col + j};
                    if (!IN_BOUNDS(c))
                        continue;
                    select_update(&sel, board, c, piece);
                }
            break;
        }
        case PIECE_KNIGHT: {
            Square map[8] = {
                {.row = row + 1, .col = col + 2},
                {.row = row - 1, .col = col + 2},
                {.row = row + 1, .col = col - 2},
                {.row = row - 1, .col = col - 2},
                {.row = row + 2, .col = col + 1},
                {.row = row - 2, .col = col + 1},
                {.row = row + 2, .col = col - 1},
                {.row = row - 2, .col = col - 1},
            };

            for (int i=0 ;i < 8; i++) {
                if (IN_BOUNDS(map[i]))
                    select_update(&sel, board, map[i], piece);
            }
           break;
        }
        case PIECE_ROOK:
        case PIECE_BISHOP:
        case PIECE_QUEEN: {
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    if (piece.type == PIECE_BISHOP && (i == 0 || j == 0)) continue;
                    if (piece.type == PIECE_ROOK && !(i == 0 || j == 0)) continue;
                    
                    for (int v = 1; v < NUM_ROWS; v++) {
                        Square sq = {.row = row + i * v, .col = col + j * v};
                        if (!IN_BOUNDS(sq) || select_update(&sel, board, sq, piece))
                            break;
                    }
                }
            break;
        }
        case PIECE_PAWN: {
            int dir = (piece.player == PLAYER_WHITE) ? 1 : -1;
            Square c = {.row = row + dir, .col = col};
            if (IN_BOUNDS(c)) {
                auto pid = BOARD2(*board, c);
                if (pid == NO_PIECE)
                    sel.possible_moves[sel.num_possible_moves++] = c;
            }

            if ((piece.player == PLAYER_WHITE && row == 1) || (piece.player == PLAYER_BLACK && row == 6)) {
                c = {.row = row + 2*dir, .col = col};
                if (IN_BOUNDS(c)) {
                    auto pid = BOARD2(*board, c);
                    if (pid == NO_PIECE)
                        sel.possible_moves[sel.num_possible_moves++] = c;
                }
            }

            auto pid = BOARD(*board, row + dir, col + 1);
            if (pid != NO_PIECE && board->pieces[pid].player != piece.player)
                sel.threatened_pieces[sel.num_threatened_pieces++] = pid;

            pid = BOARD(*board, row + dir, col - 1);
            if (pid != NO_PIECE && board->pieces[pid].player != piece.player)
                sel.threatened_pieces[sel.num_threatened_pieces++] = pid;

            break;
        }
        default:
            return select_none;
    }
    
    return sel;
}