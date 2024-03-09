#include "move.h"

#define PIECE_SELECTED(select) (BOARD2(*((select)->board), (select)->pos) != NO_PIECE)

Selection select_tile_ignore_check(Board *board, int row, int col);

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

bool is_move_possible(Selection select, int row, int col) {
    for (int i = 0; i < select.num_possible_moves; i++)
        if (select.possible_moves[i].row == row && select.possible_moves[i].col == col)
            return true;
    return false;
}

Board move_selected_piece(Selection *select, int row, int col) {
    Board board = *select->board;
    if (!PIECE_SELECTED(select))
        return board;
    BOARD(board, row, col) = BOARD2(board, select->pos);
    BOARD2(board, select->pos) = NO_PIECE;

    // pawn promotion
    if (row == 0 || row == 7) {
        auto pid = BOARD(board, row, col);
        if (board.pieces[pid].type == PIECE_PAWN)
            board.pieces[pid].type = PIECE_QUEEN;
    }
    
    return board;
}

Piece get_selected_piece(Selection *select) {
    auto pid = BOARD2(*select->board, select->pos);
    if (pid == NO_PIECE)
        return {.type = PIECE_NONE, .player = PLAYER_NONE};
    return select->board->pieces[pid];
}

int checking_pieces(pid *checking, Board *board, Player player) {
    int count = 0;
    for (int i = 0; i < NUM_ROWS; i++)
        for (int j = 0; j < NUM_COLS; j++) {
            auto pid = BOARD(*board, i, j);
            if (pid != NO_PIECE && board->pieces[pid].player == OPPONENT_OF(player)) {
                printf("[%d %d %d]\n", i ,j, board == NULL);
                auto select = select_tile_ignore_check(board, i, j);
                for (int t = 0; t < select.num_threatened_pieces; t++) {
                    if (board->pieces[select.threatened_pieces[t]].type == PIECE_KING) {
                        if (checking != NULL)
                            checking[count++] = pid;
                        else
                            count++;
                    }
                }
            }
        }
    return count;
}

void delete_possible_moves_due_to_check(Selection *select) {
    auto player = get_selected_piece(select).player;

    if (checking_pieces(NULL, select->board, player) == 0)
        return;
    
    for (int i = 0; i < select->num_possible_moves; i++) {
        Board hypo_board = move_selected_piece(select, select->possible_moves[i].row, select->possible_moves[i].col);
        if (checking_pieces(NULL, &hypo_board, player) > 0) {
            select->possible_moves[i] = NO_SQUARE;
        }
    }

}

Selection select_tile_ignore_check(Board *board, int row, int col) {
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
            if (pid != NO_PIECE && board->pieces[pid].player != piece.player) {
                sel.possible_moves[sel.num_possible_moves++] = {.row = row + dir, .col = col + 1};
                sel.threatened_pieces[sel.num_threatened_pieces++] = pid;
            }

            pid = BOARD(*board, row + dir, col - 1);
            if (pid != NO_PIECE && board->pieces[pid].player != piece.player) {
                sel.possible_moves[sel.num_possible_moves++] = {.row = row + dir, .col = col - 1};
                sel.threatened_pieces[sel.num_threatened_pieces++] = pid;
            }
            break;
        }
        default:
            return select_none;
    }
    return sel;

}
Selection select_tile(Board *board, int row, int col) {
    Selection select = select_tile_ignore_check(board, row, col);
    delete_possible_moves_due_to_check(&select);
    return select;
}