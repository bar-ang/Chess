#include "move.h"

#define PIECE_SELECTED(select) (BOARD2(*((select)->board), (select)->pos) != NO_PIECE)

Selection select_tile_ignore_check(Board *board, int row, int col);

bool select_update(Selection *sel, Board *board, Tile c, Piece piece) {
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

Selection unselect(Board *board) {
    Selection sel;
    sel.board = board;
    sel.num_possible_moves = 0;
    sel.num_threatened_pieces = 0;
    sel.pos = NO_TILE;

    return sel;
}

Board move_selected_piece(Selection *select, int row, int col) {
    Board board = *select->board;
    if (!PIECE_SELECTED(select))
        return board;
    BOARD(board, row, col) = BOARD2(board, select->pos);
    BOARD2(board, select->pos) = NO_PIECE;

    auto pid = BOARD(board, row, col);
    board.num_times_piece_has_moved[pid]++;

    // pawn promotion
    if (row == 0 || row == 7) {
        if (board.pieces[pid].type == PIECE_PAWN)
            board.pieces[pid].type = PIECE_QUEEN;
    }

    // casteling
    if (board.pieces[pid].type == PIECE_KING) {
        if (col - select->pos.col == 2) {
            board.num_times_piece_has_moved[BOARD(board, row, 7)]++;
            BOARD(board, row, col-1) = BOARD(board, row, 7);
            BOARD(board, row, 7) = NO_PIECE;
        } else if (col - select->pos.col == -2) {
            board.num_times_piece_has_moved[BOARD(board, row, 0)]++;
            BOARD(board, row, col+1) = BOARD(board, row, 0);
            BOARD(board, row, 0) = NO_PIECE;
        }
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

// *** assumes the player is in check, otherwise the function behaves badly! **
// 'resolvers' is an array of pieces that can get king out of check.
// the return value is the number of resolvers.
// if is_in_checkmate(...) == 0 then it is checkmate.
int is_in_checkmate(Board *board, Player player, pid *resolvers) {
    int count = 0;
    for (int i = 0; i < NUM_ROWS; i++)
        for (int j = 0; j < NUM_COLS; j++) {
            auto pid = BOARD(*board, i, j);
            if (pid == NO_PIECE)
                continue;
            if (board->pieces[pid].player == player) {
                auto select = select_tile(board, i, j);
                if (select.num_possible_moves > 0) {
                    if (resolvers != NULL)
                        resolvers[count++] = pid;
                    else
                        count++;
                }
            }
        }
    return count;
}

Player checkmate(Board *board) {
    if (checking_pieces(NULL, board, PLAYER_WHITE) > 0 && is_in_checkmate(board, PLAYER_WHITE, NULL) == 0)
        return PLAYER_WHITE;
    else if (checking_pieces(NULL, board, PLAYER_BLACK) > 0 && is_in_checkmate(board, PLAYER_BLACK, NULL) == 0)
        return PLAYER_BLACK;
    else
        return PLAYER_NONE;
}

void delete_possible_moves_due_to_check(Selection *select) {
    auto player = get_selected_piece(select).player;

    if (checking_pieces(NULL, select->board, player) == 0)
        return;
    
    for (int i = 0; i < select->num_possible_moves; i++) {
        Board hypo_board = move_selected_piece(select, select->possible_moves[i].row, select->possible_moves[i].col);
        if (checking_pieces(NULL, &hypo_board, player) > 0) {
            select->possible_moves[i] = NO_TILE;
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
        return unselect(board);
    auto piece = board->pieces[pid];
    switch (piece.type) {
        case PIECE_KING: {
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    Tile c = {.row = row + i, .col = col + j};
                    if (!IN_BOUNDS(c))
                        continue;
                    select_update(&sel, board, c, piece);
                }

            // Casteling
            if (board->num_times_piece_has_moved[pid] == 0) {
                auto rook = BOARD(*board, row, 0);
                if (rook == PIECE_ROOK && board->pieces[rook].player == piece.player && board->num_times_piece_has_moved[rook] == 0) {
                    for (int t = 0; t < col; t++)
                        if (BOARD(*board, row, t) != NO_PIECE)
                            goto second_rook;
                    select_update(&sel, board, {.row = row, .col = col - 2}, piece);
                }
second_rook:                
                rook = BOARD(*board, row, 7);
                if (rook == PIECE_ROOK && board->pieces[rook].player == piece.player && board->num_times_piece_has_moved[rook] == 0) {
                    for (int t = col+1; t < 8; t++)
                        if (BOARD(*board, row, t) != NO_PIECE)
                            goto end;
                    select_update(&sel, board, {.row = row, .col = col + 2}, piece);
                }
            }
end:            
            break;
        }
        case PIECE_KNIGHT: {
            Tile map[8] = {
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
                        Tile sq = {.row = row + i * v, .col = col + j * v};
                        if (!IN_BOUNDS(sq) || select_update(&sel, board, sq, piece))
                            break;
                    }
                }
            break;
        }
        case PIECE_PAWN: {
            int dir = (piece.player == PLAYER_WHITE) ? 1 : -1;
            Tile c = {.row = row + dir, .col = col};
            if (IN_BOUNDS(c)) {
                auto pid = BOARD2(*board, c);
                if (pid == NO_PIECE)
                    sel.possible_moves[sel.num_possible_moves++] = c;
            }

            if ((piece.player == PLAYER_WHITE && row == 1) || (piece.player == PLAYER_BLACK && row == 6)) {
                c = {.row = row + 2*dir, .col = col};
                if (IN_BOUNDS(c) && BOARD(*board, row + dir, col) == NO_PIECE) {
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
            return unselect(board);
    }
    return sel;

}
Selection select_tile(Board *board, int row, int col) {
    Selection select = select_tile_ignore_check(board, row, col);
    delete_possible_moves_due_to_check(&select);
    return select;
}