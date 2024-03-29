#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

Player search_for_check(Board);

Board set_piece(Board board, pid p_id, PieceType type, Player player, int row, int col) {
    board.pieces[p_id].type = type;
    board.pieces[p_id].player = player;
    board.num_times_piece_has_moved[p_id] = 0;
    BOARD(board, row, col) = p_id;
    return board;
}

Board init_empty_board() {
    Board board;

    for (int i = 0; i<NUM_TILES; i++)
        board.board[i] = NO_PIECE;
    return board;
}

Board init_board() {
    Board board = init_empty_board();
    int i = 0;

#if SET_KINGS || !DEBUG_MODE
    board = set_piece(board, i++, PIECE_KING, PLAYER_WHITE, 0, 4);
    board = set_piece(board, i++, PIECE_KING, PLAYER_BLACK, 7, 4);
#endif
#if SET_QUEENS || !DEBUG_MODE
    board = set_piece(board, i++, PIECE_QUEEN, PLAYER_WHITE, 0, 3);
    board = set_piece(board, i++, PIECE_QUEEN, PLAYER_BLACK, 7, 3);
#endif
#if SET_BISHOPS || !DEBUG_MODE
    board = set_piece(board, i++, PIECE_BISHOP, PLAYER_WHITE, 0, 2);
    board = set_piece(board, i++, PIECE_BISHOP, PLAYER_WHITE, 0, 5);
    board = set_piece(board, i++, PIECE_BISHOP, PLAYER_BLACK, 7, 2);
    board = set_piece(board, i++, PIECE_BISHOP, PLAYER_BLACK, 7, 5);
#endif
#if SET_KNIGHTS || !DEBUG_MODE
    board = set_piece(board, i++, PIECE_KNIGHT, PLAYER_WHITE, 0, 1);
    board = set_piece(board, i++, PIECE_KNIGHT, PLAYER_WHITE, 0, 6);
    board = set_piece(board, i++, PIECE_KNIGHT, PLAYER_BLACK, 7, 1);
    board = set_piece(board, i++, PIECE_KNIGHT, PLAYER_BLACK, 7, 6);
#endif
#if SET_ROOKS || !DEBUG_MODE
    board = set_piece(board, i++, PIECE_ROOK, PLAYER_WHITE, 0, 0);
    board = set_piece(board, i++, PIECE_ROOK, PLAYER_WHITE, 0, 7);
    board = set_piece(board, i++, PIECE_ROOK, PLAYER_BLACK, 7, 0);
    board = set_piece(board, i++, PIECE_ROOK, PLAYER_BLACK, 7, 7);
#endif

#if SET_PAWNS || !DEBUG_MODE
    for (int j = 0; j < 8; j++) {
        board = set_piece(board, i++, PIECE_PAWN, PLAYER_WHITE, 1, j);
        board = set_piece(board, i++, PIECE_PAWN, PLAYER_BLACK, 6, j);
    }
#endif
    
    return board;
}

bool compare_boards(Board *board1, Board *board2) {
    return memcmp(board1, board2, sizeof(Board)) == 0;
}

int player_score(Board *board, Player player) {
    int score = 0;
    for (int i = 0; i < NUM_ROWS; i++)
        for (int j = 0; j < NUM_COLS; j++) {
            auto pid = BOARD(*board, i, j);
            if (pid != NO_PIECE && board->pieces[pid].player == player)
                score += piece_score(board->pieces[pid]);
        }

    return score;
}

int board_score(Board *board) {
    int score = 0;
    for (int i = 0; i < NUM_ROWS; i++)
        for (int j = 0; j < NUM_COLS; j++) {
            auto pid = BOARD(*board, i, j);
            if (pid != NO_PIECE)
                score += piece_score(board->pieces[pid]) * (board->pieces[pid].player == PLAYER_WHITE ? 1 : -1);
        }

    return score;
}