#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "move.h"

#define PIECE_SELECTED(select) (BOARD2(*((select)->board), (select)->pos) != NO_PIECE)

Selection select_tile_ignore_check(Board *, int, int);

int num_possible_moves(Selection *select) {
    int c = 0;
    for (auto m = select->possible_moves; m < select->possible_moves + select->possible_moves_len; m++) {
        ASSERT(m->row <= NUM_ROWS && m->col <= NUM_COLS);
        if (m->row > 0 && m->col > 0)
            c++;
    }
    return c;
}

bool select_update(Selection *sel, Board *board, Tile c, Piece piece) {
    auto pid = BOARD2(*board, c);
    ASSERTE(c.row >= 0 && c.row <= NUM_ROWS && c.col >= 0 && c.col <= NUM_COLS, printf("update (%d, %d)\n" , c.row, c.col))
    if (pid == NO_PIECE)
        sel->possible_moves[sel->possible_moves_len++] = c;
    else {
        auto p2 = board->pieces[pid];
        if (piece.player != p2.player) {
            if (piece.type != PIECE_KING)
                sel->possible_moves[sel->possible_moves_len++] = c;
            sel->threatened_pieces[sel->num_threatened_pieces++] = pid;
        }
        return true;
    }
    return false;
}

bool is_move_possible(Selection *select, int row, int col) {
    ASSERT(row >= 0 && row <= NUM_ROWS && col >= 0 && col <= NUM_COLS)
    for (int i = 0; i < select->possible_moves_len; i++)
        if (select->possible_moves[i].row == row && select->possible_moves[i].col == col)
            return true;
    return false;
}

Selection unselect(Board *board) {
    Selection sel;
    sel.board = board;
    sel.possible_moves_len = 0;
    sel.num_threatened_pieces = 0;
    sel.pos = NO_TILE;

    return sel;
}

Board move_selected_piece(Selection *select, int row, int col, pid *eaten_piece) {
    auto board = *select->board;
    if (!PIECE_SELECTED(select))
        return board;

#if DEBUG_MODE
    ASSERTE(row >= 0 && row <= NUM_ROWS && col >= 0 && col <= NUM_COLS, printf("moving (%d, %d) -> (%d, %d)\n", select->pos.row, select->pos.col, row, col))
    ASSERT(row != select->pos.row || col != select->pos.col);
    bool onboard[NUM_PIECES];
    memset(onboard, 0, sizeof(onboard));

    for (int i =0 ;i < NUM_TILES; i++) {
        if (board.board[i] != NO_PIECE) {
            ASSERT(!onboard[board.board[i]]);
            onboard[board.board[i]] = true;
        }
    }
#endif

    ASSERT(is_move_possible(select, row, col));

    auto pid = BOARD2(board, select->pos);
#if DEBUG_MODE
    auto d_eaten_piece = BOARD(board, row, col);
#endif
    if (eaten_piece != NULL)
        *eaten_piece = BOARD(board, row, col);

    BOARD(board, row, col) = pid;
    BOARD2(board, select->pos) = NO_PIECE;

    ASSERT(pid != NO_PIECE);

    board.num_times_piece_has_moved[pid]++;

    // pawn promotion
    if ((row == 0 || row == 7) && board.pieces[pid].type == PIECE_PAWN) {
        ASSERT((board.pieces[pid].player == PLAYER_BLACK && row == 0) || (board.pieces[pid].player == PLAYER_WHITE && row == 7))
        board.pieces[pid].type = PIECE_QUEEN;
    }

    // casteling
    if (board.pieces[pid].type == PIECE_KING) {
#if DEBUG_MODE
        auto score = board_score(&board);
#endif
        if (col - select->pos.col == 2) {
            ASSERT(board.pieces[BOARD(board, row, 7)].type == PIECE_ROOK)
            board.num_times_piece_has_moved[BOARD(board, row, 7)]++;
            BOARD(board, row, col-1) = BOARD(board, row, 7);
            BOARD(board, row, 7) = NO_PIECE;
        } else if (col - select->pos.col == -2) {
            ASSERT(board.pieces[BOARD(board, row, 0)].type == PIECE_ROOK)
            board.num_times_piece_has_moved[BOARD(board, row, 0)]++;
            BOARD(board, row, col+1) = BOARD(board, row, 0);
            BOARD(board, row, 0) = NO_PIECE;
        }
        ASSERT(board_score(&board) == score);
    }
    
#if DEBUG_MODE
    ASSERT(memcmp(board.board, select->board->board, sizeof(board.board)) != 0);
    ASSERT(d_eaten_piece == NO_PIECE || board.pieces[d_eaten_piece].type != PIECE_KING);
    bool onboard2[NUM_PIECES];
    memset(onboard2, 0, sizeof(onboard2));

    for (int i =0 ;i < NUM_TILES; i++) {
        if (board.board[i] != NO_PIECE) {
            ASSERT(!onboard2[board.board[i]]);
            ASSERTE(board.board[i] != d_eaten_piece, printf("%d is still on board at %d although eaten\n", d_eaten_piece, i));
            onboard2[board.board[i]] = true;
        }
    }

    for (int i = 0 ;i < NUM_PIECES; i++) {
            if (i != d_eaten_piece) {
                ASSERTE(onboard[i] == onboard2[i], printf("%d is inconsistent. was? %d, now? %d\n", i, onboard[i], onboard2[i]));
            }
            else {
                ASSERTE(onboard[i] && !onboard2[i], printf("%d is inconsistent. was? %d eaten? %d (eaten piece: %d)\n", i, onboard[i], !onboard2[i], d_eaten_piece));
            }
    }
#endif
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
                if (select.possible_moves_len > 0) {
                    if (resolvers != NULL)
                        resolvers[count++] = pid;
                    else
                        count++;
                }
            }
        }
    return count;
}

bool player_can_move(Board *board, Player player) {
    for (int i = 0; i < NUM_ROWS; i++)
        for (int j = 0; j < NUM_COLS; j++) {
            auto pid = BOARD(*board, i, j);
            if (pid != NO_PIECE && board->pieces[pid].player == player) {
                auto select = select_tile(board, i, j);
                if (num_possible_moves(&select) > 0)
                    return true;
            }
        }

    return false;
}

bool stalemate_by_agreement(Board *board) {
    return false;
}

bool stalemate_by_repetition(Board *board) {
    return false;
}

bool stalemate(Board *board) {
    return  stalemate_by_agreement(board) ||
            stalemate_by_repetition(board) ||
            !player_can_move(board, PLAYER_BLACK) ||
            !player_can_move(board, PLAYER_WHITE);
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

    if (player == PLAYER_NONE) {
        FAIL;
        return;
    }

    if (checking_pieces(NULL, select->board, player) == 0)
        return;
    
    for (int i = 0; i < select->possible_moves_len; i++) {
        ASSERT(select->possible_moves[i].row >= 0 && select->possible_moves[i].col >= 0)
        Board hypo_board = move_selected_piece(select, select->possible_moves[i].row, select->possible_moves[i].col, NULL);
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
    sel.possible_moves_len = 0;
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

#if !DEBUG_MODE || !DISABLE_CASTELING_FOR_DEBUG
            // Casteling
            if (board->num_times_piece_has_moved[pid] == 0) {
                ASSERT((piece.player == PLAYER_WHITE && row == 0) || (piece.player == PLAYER_BLACK && row == NUM_COLS - 1))
                auto rook = BOARD(*board, row, 0);
                if (rook == PIECE_ROOK && board->pieces[rook].player == piece.player && board->num_times_piece_has_moved[rook] == 0) {
                    for (int t = 0; t < col; t++)
                        if (BOARD(*board, row, t) != NO_PIECE)
                            goto second_rook;
                    select_update(&sel, board, {.row = row, .col = col - 2}, piece);
                }
second_rook:                
                rook = BOARD(*board, row, NUM_COLS-1);
                if (rook == PIECE_ROOK && board->pieces[rook].player == piece.player && board->num_times_piece_has_moved[rook] == 0) {
                    for (int t = col+1; t < NUM_COLS; t++)
                        if (BOARD(*board, row, t) != NO_PIECE)
                            goto end;
                    select_update(&sel, board, {.row = row, .col = col + 2}, piece);
                }
            }
end:
#endif
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
                    sel.possible_moves[sel.possible_moves_len++] = c;
            }

            if ((piece.player == PLAYER_WHITE && row == 1) || (piece.player == PLAYER_BLACK && row == 6)) {
                c = {.row = row + 2*dir, .col = col};
                if (IN_BOUNDS(c) && BOARD(*board, row + dir, col) == NO_PIECE) {
                    auto pid = BOARD2(*board, c);
                    if (pid == NO_PIECE)
                        sel.possible_moves[sel.possible_moves_len++] = c;
                }
            }

            auto pid = BOARD(*board, row + dir, col + 1);
            if (pid != NO_PIECE && board->pieces[pid].player != piece.player) {
                Tile d = {.row = row + dir, .col = col + 1};
                if (IN_BOUNDS(d)) {
                    ASSERT(row + dir >= 0 && row + dir < NUM_ROWS && col + 1 >= 0 && col + 1 < NUM_COLS)
                    if (board->pieces[pid].type != PIECE_KING)
                        sel.possible_moves[sel.possible_moves_len++] = d;
                    sel.threatened_pieces[sel.num_threatened_pieces++] = pid;
                }
            }

            pid = BOARD(*board, row + dir, col - 1);
            if (pid != NO_PIECE && board->pieces[pid].player != piece.player) {
                Tile d = {.row = row + dir, .col = col - 1};
                if (IN_BOUNDS(d)) {
                    ASSERT(row + dir >= 0 && row + dir < NUM_ROWS && col - 1 >= 0 && col - 1 < NUM_COLS)
                    if (board->pieces[pid].type != PIECE_KING)
                        sel.possible_moves[sel.possible_moves_len++] = {.row = row + dir, .col = col - 1};
                    sel.threatened_pieces[sel.num_threatened_pieces++] = pid;
                }
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

Board random_move_for_piece(Board board, int row, int col, Tile *to, bool *success) {
    auto select = select_tile(&board, row, col);
    if (num_possible_moves(&select) == 0) {
        *success = false;
        return board;
    }

    Tile tile;
    int rnd;
    do {
        rnd = rand() % select.possible_moves_len;
        tile = select.possible_moves[rnd];
    } while (tile.row < 0 || tile.col < 0); //remeber there are moves that were deleted due to check!

    *success = true;
    *to = tile;
    return move_selected_piece(&select, tile.row, tile.col, NULL);
}

Board random_move(Board board, Player player, Tile *from, Tile *to) {
    pid rnd;
    bool success = false;
    Board new_board;

    while(!success) {
        do {
            rnd = (pid)(rand() % NUM_PIECES);
        } while (board.pieces[rnd].player != player);
    
        for (int i = 0; i < NUM_ROWS; i++)
            for (int j = 0; j < NUM_COLS; j++)
                if (BOARD(board, i, j) == rnd) {
                    new_board = random_move_for_piece(board, i, j, to, &success);
                    *from = {.row = i, .col = j};
                    break;
                }
    }

    return new_board;
}

int adjacent_boards(Board *board, Player player, int i, int j, Board *adj_boards) {
    int num_adj_boards = 0;

    Selection select;
    auto pid = BOARD(*board, i, j);
    if (pid == NO_PIECE || board->pieces[pid].player != player)
        return 0;
    select = select_tile(board, i, j);
    for (int p = 0; p < select.possible_moves_len; p++)
        adj_boards[num_adj_boards++] = move_selected_piece(&select, select.possible_moves[p].row, select.possible_moves[p].col, NULL);
    
    return num_adj_boards;
}