#include <stdio.h>
#include "printer.h"

typedef int color;

bool sign(char *str, PieceType type) {
    switch (type) {
        case PIECE_KING:
            sprintf(str, "K");
            break;
        case PIECE_QUEEN:
            sprintf(str, "Q");
            break;
        case PIECE_KNIGHT:
            sprintf(str, "N");
            break;
        case PIECE_BISHOP:
            sprintf(str, "B");
            break;
        case PIECE_ROOK:
            sprintf(str, "R");
            break;
        case PIECE_PAWN:
            sprintf(str, "P");
            break;
        default:
            return false;
    }
    return true;
}

void print_selection(Selection select) {
    /*
        bool found;
        char str[60];
        color bgcolor;
        Selection select;
        int num_moves = 0;
        if (board.selected_piece >= 0) {
            num_moves = possible_moves(selected, board, board.selected_piece);
        }
        for (int i = 0; i < NUM_ROWS; i++) {
            for (int j = 0; j < NUM_COLS; j++) {
                bgcolor = ((i+j) % 2) ? COLOR_BG_GRAY : COLOR_BG_BLACK;
                if (board.selected_piece >= 0) {
                    auto p = board.pieces[board.selected_piece];
                    if (p.loc.row == i && p.loc.col == j)
                        bgcolor = COLOR_BG_CYAN;
                    for(int k = 0; k < num_moves; k++) {
                        if (selected[k].row == i && selected[k].col == j)
                            bgcolor = COLOR_BG_YELLOW;
                    }
                }
                found = false;
                for (Piece *p = board.pieces; p < board.pieces + board.num_pieces; p++) {
                    if (p->loc.row == i && p->loc.col == j) {
                        if (!found) {
                            found = sign(str, p->type);
                            PRINTCOLOR(p->player == PLAYER_WHITE ? COLOR_RED : COLOR_BLUE, bgcolor);
                            break;
                        }
                    }
                }
                if (!found) {
                    sprintf(str, "%c", EMPTY);
                    PRINTCOLOR(0, bgcolor);
                }

                printf(" %s ", str);
                RESETCOLOR;
            }
            printf("\n");
        }
*/
}

void print_board(Board board) {
    char str[10];
    color bgcolor;
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            bgcolor = ((i+j) % 2) ? WHITE_TILE_BGCOLOR : BLACK_TILE_BGCOLOR;
            auto pid = BOARD(board, i, j);
            if (pid == NO_PIECE) {
                PRINTCOLOR(NO_COLOR, bgcolor); 
                sprintf(str, "%c", EMPTY);
            } else {
                auto piece = board.pieces[pid];
                sign(str, piece.type);
                PRINTCOLOR(piece.player == PLAYER_WHITE ? WHITE_PLAYER_COLOR : BLACK_PLAYER_COLOR, bgcolor);
            }
            printf(" %s ", str);
            RESETCOLOR;
        }
        printf("\n");
    }
}

void print_board_code(Board board) {
    for (int i = 0; i < sizeof(board); i++) {
        printf("%02x", ((char *)&board)[i]);
    }
    printf("\n");
}