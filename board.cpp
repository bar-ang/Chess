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

int threatened_pieces(int *threatened, Board board, int piece) {
    int count = 0;
    Cell moves[MAX_NUM_PIECES];
    int num_moves = possible_moves(moves, board, piece);
    for (int i = 0; i < num_moves; i++) {
        int j = get_piece(board, moves[i].row, moves[i].col);
        if (j < 0) continue;
        auto p = board.pieces[j];
        if (p.player != board.pieces[piece].player)
            threatened[count++] = j;
    }

    return count;
}

bool allowed_to_move(Board board, int piece, Cell cell) {
    return path_is_accessible(board, board.pieces[piece], cell.row, cell.col);
}

Board remove_eaten_piece_if_needed(Board board, int piece, Cell cell) {
    for (int i=0; i < board.num_pieces; i++) {
        auto p = board.pieces + i;
        if (i != piece && p->loc.row == cell.row && p->loc.col == cell.col) {
            p->type = PIECE_NONE;
        }
    }

    return board;
}

Player checkmate(Board board) {
    return board.checkmate;
}

bool is_in_checkmate(Board board, int piece) {
    Cell moves[8];
    int num_moves = possible_moves(moves, board, piece);
    for (int i = 0; i < num_moves; i++) {
        auto b = move_hypothetical(board, piece, moves[i]);
        if (search_for_check(b) != board.pieces[piece].player)
            return false;
    }

    return true;
}

Player search_for_check(Board board) {
    for (int i = 0; i < board.num_pieces; i++) {
        int threatened[MAX_NUM_PIECES];
        int num_threatened = threatened_pieces(threatened, board, i);
        for (int j = 0; j < num_threatened; j++) {
            auto p = board.pieces[threatened[j]];
            if (p.type == PIECE_KING) {
                return p.player;
            }
        }
    }

    return PLAYER_NONE;
}

Board move_hypothetical(Board board, int piece, Cell cell) {
    auto p = board.pieces + piece;
    p->loc = cell;
    board = remove_eaten_piece_if_needed(board, piece, cell);

    return board;
}

Board move(Board board, int piece, Cell cell) {
    board = move_hypothetical(board, piece, cell);
    board.check = search_for_check(board);
    if (board.check != PLAYER_NONE)
        board.checkmate = is_in_checkmate(board, piece) ? board.check : PLAYER_NONE;
    return board;
}