#include "board.h"

Board set_piece(Board board, PieceType type, Player player, int row, int col) {
    board.pieces[board.num_pieces].type = type;
    board.pieces[board.num_pieces].player = player;
    board.pieces[board.num_pieces].row = row;
    board.pieces[board.num_pieces].col = col;
    board.num_pieces++;
    return board;
}

Board init_board() {
    Board board;
    board.num_pieces = 0;

    board = set_piece(board, PIECE_ROOK, PLAYER_WHITE, 0, 0);
    board = set_piece(board, PIECE_KNIGHT, PLAYER_WHITE, 0, 1);
    board = set_piece(board, PIECE_BISHOP, PLAYER_WHITE, 0, 2);
    board = set_piece(board, PIECE_QUEEN, PLAYER_WHITE, 0, 3);
    board = set_piece(board, PIECE_KING, PLAYER_WHITE, 0, 4);
    board = set_piece(board, PIECE_BISHOP, PLAYER_WHITE, 0, 5);
    board = set_piece(board, PIECE_KNIGHT, PLAYER_WHITE, 0, 6);
    board = set_piece(board, PIECE_ROOK, PLAYER_WHITE, 0, 7);

    board = set_piece(board, PIECE_ROOK, PLAYER_BLACK, 7, 0);
    board = set_piece(board, PIECE_KNIGHT, PLAYER_BLACK, 7, 1);
    board = set_piece(board, PIECE_BISHOP, PLAYER_BLACK, 7, 2);
    board = set_piece(board, PIECE_QUEEN, PLAYER_BLACK, 7, 3);
    board = set_piece(board, PIECE_KING, PLAYER_BLACK, 7, 4);
    board = set_piece(board, PIECE_BISHOP, PLAYER_BLACK, 7, 5);
    board = set_piece(board, PIECE_KNIGHT, PLAYER_BLACK, 7, 6);
    board = set_piece(board, PIECE_ROOK, PLAYER_BLACK, 7, 7);

    for (int i = 0; i < 8; i++) {
        board = set_piece(board, PIECE_PAWN, PLAYER_WHITE, 1, i);
        board = set_piece(board, PIECE_PAWN, PLAYER_BLACK, 6, i);
    }
    
    return board;
}