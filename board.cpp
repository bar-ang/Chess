#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

Player search_for_check(Board);

Board set_piece(Board board, pid p_id, PieceType type, Player player, int row, int col) {
    board.pieces[p_id].type = type;
    board.pieces[p_id].player = player;
    BOARD(board, row, col) = p_id;
    return board;
}

Board init_board() {
    Board board;
    board.selected_square = {-1, -1};
    int i = 0;

    for (int i = 0; i<NUM_SQUARES; i++)
        board.board[i] = NO_PIECE;
    
    board = set_piece(board, i++, PIECE_ROOK, PLAYER_WHITE, 0, 0);
    board = set_piece(board, i++, PIECE_KNIGHT, PLAYER_WHITE, 0, 1);
    board = set_piece(board, i++, PIECE_BISHOP, PLAYER_WHITE, 0, 2);
    board = set_piece(board, i++, PIECE_QUEEN, PLAYER_WHITE, 0, 3);
    board = set_piece(board, i++, PIECE_KING, PLAYER_WHITE, 0, 4);
    board = set_piece(board, i++, PIECE_BISHOP, PLAYER_WHITE, 0, 5);
    board = set_piece(board, i++, PIECE_KNIGHT, PLAYER_WHITE, 0, 6);
    board = set_piece(board, i++, PIECE_ROOK, PLAYER_WHITE, 0, 7);

    board = set_piece(board, i++, PIECE_ROOK, PLAYER_BLACK, 7, 0);
    board = set_piece(board, i++, PIECE_KNIGHT, PLAYER_BLACK, 7, 1);
    board = set_piece(board, i++, PIECE_BISHOP, PLAYER_BLACK, 7, 2);
    board = set_piece(board, i++, PIECE_QUEEN, PLAYER_BLACK, 7, 3);
    board = set_piece(board, i++, PIECE_KING, PLAYER_BLACK, 7, 4);
    board = set_piece(board, i++, PIECE_BISHOP, PLAYER_BLACK, 7, 5);
    board = set_piece(board, i++, PIECE_KNIGHT, PLAYER_BLACK, 7, 6);
    board = set_piece(board, i++, PIECE_ROOK, PLAYER_BLACK, 7, 7);

    for (int j = 0; j < 8; j++) {
        board = set_piece(board, i++, PIECE_PAWN, PLAYER_WHITE, 1, j);
        board = set_piece(board, i++, PIECE_PAWN, PLAYER_BLACK, 6, j);
    }
    
    return board;
}

Player checkmate(Board board) {
    return PLAYER_NONE;
}

Board random_move_for_piece(Board board, int piece) {
    return board;
}

Board random_move(Board board, Player player) {
    auto original = board;
    int piece;
    while (memcmp(&original, &board, sizeof(board)) == 0) {
        do {
            piece = rand() % NUM_PIECES;
        } while (board.pieces[piece].player != player);
        board = random_move_for_piece(board, piece);
    }
    return board;
}