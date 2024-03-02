#include "board.h"

Board set_piece(Board board, PieceType type, Player player, int row, int col) {
    board.pieces[board.num_pieces].type = type;
    board.pieces[board.num_pieces].player = player;
    board.pieces[board.num_pieces].loc.row = row;
    board.pieces[board.num_pieces].loc.col = col;
    board.num_pieces++;
    return board;
}

Board init_board() {
    Board board;
    board.num_pieces = 0;
    board.selected_piece = -1;

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

int get_piece(Board board, int row, int col) {
    for (int i = 0; i < board.num_pieces; i++) {
        if (board.pieces[i].loc.row == row && board.pieces[i].loc.col == col)
            return i;
    }
    return -1;
}

Board scrambled_board() {
    Board board;
    return board;
}

bool path_is_accessible(Board board, Piece piece, int row, int col) {
    int p;
    switch (piece.type) {
        case PIECE_KING:
        case PIECE_KNIGHT: {
            p = get_piece(board, row, col);
            if (p >= 0)
                return board.pieces[p].player != piece.player;
            return true;
        }
        case PIECE_ROOK:
        case PIECE_BISHOP:
        case PIECE_QUEEN: {
            int dirx = row - piece.loc.row;
            int diry = col - piece.loc.col;
            if (dirx < 0) dirx = -1;
            if (dirx > 0) dirx = 1;
            if (diry < 0) diry = -1;
            if (diry > 0) diry = 1;
            for (int r = piece.loc.row, c = piece.loc.col; r != row && c != col; r += dirx, c += diry) {
                if (get_piece(board, r, c) >= 0) return false;
            }

            p = get_piece(board, row, col);
            if (p >= 0)
                return board.pieces[p].player != piece.player;
            return true;
        }
        case PIECE_PAWN: {
            if (get_piece(board, row, col) >= 0) return false;
            if (row - piece.loc.row >= 2 || row - piece.loc.row <= -2) return false;
            return true;
        }
        default: {
            return false;
        }
    }
}

int possible_moves(Cell *moves, Board board, int piece) {
    Cell map[NUM_CELLS];
    auto p =  board.pieces[piece];
    int orig_count = move_map(map, p);
    int count = 0;

    for (Cell *c = map; c < map + orig_count; c++) {
        if (!path_is_accessible(board,  p, c->row, c->col)) continue;
        moves[count++] = *c;
    }

    return count;
}