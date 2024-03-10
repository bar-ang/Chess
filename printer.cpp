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
    if (!select.board) {
        printf(">>> NO BOARD <<<\n");
        return;
    }
    char str[10];
    color bgcolor;

    PRINTCOLOR(EDGE_COLOR, EDGE_BGCOLOR);
    printf("  ");
    for (int j = 0; j < NUM_COLS; j++) {
        printf(" %c ", 'A' + j);
    }
    printf("  ");
    RESETCOLOR;
    printf("\n");
    
    for (int i = 0; i < NUM_ROWS; i++) {
        PRINTCOLOR(EDGE_COLOR, EDGE_BGCOLOR);
        printf("%d ", i+1);
        RESETCOLOR;
        for (int j = 0; j < NUM_COLS; j++) {
            bgcolor = ((i+j) % 2) ? WHITE_TILE_BGCOLOR : BLACK_TILE_BGCOLOR;
            if (select.pos.row == i && select.pos.col == j)
                bgcolor = SELECTED_PIECE_BGCOLOR;
            for (int k = 0; k < select.num_possible_moves; k++)
                if (select.possible_moves[k].row == i && select.possible_moves[k].col == j)
                    bgcolor = POSSIBLE_MOVE_BGCOLOR;
            for (int k = 0; k < select.num_threatened_pieces; k++)
                if (select.threatened_pieces[k] == BOARD(*select.board, i, j))
                    bgcolor = THREATENED_PIECES_BGCOLOR;

            auto pid = BOARD(*select.board, i, j);
            if (pid == NO_PIECE) {
                PRINTCOLOR(NO_COLOR, bgcolor); 
                sprintf(str, "%c", EMPTY);
            } else {
                auto piece = select.board->pieces[pid];
                sign(str, piece.type);
                PRINTCOLOR(piece.player == PLAYER_WHITE ? WHITE_PLAYER_COLOR : BLACK_PLAYER_COLOR, bgcolor);
            }
            printf(" %s ", str);
            RESETCOLOR;
        }
        PRINTCOLOR(EDGE_COLOR, EDGE_BGCOLOR);
        printf("%d ", i+1);
        RESETCOLOR;
        printf("\n");
    }

    PRINTCOLOR(EDGE_COLOR, EDGE_BGCOLOR);
    printf("  ");
    for (int j = 0; j < NUM_COLS; j++) {
        printf(" %c ", 'A' + j);
    }
    printf("  ");
    RESETCOLOR;
    printf("\n");
}

void print_board(Board board) {
    print_selection(unselect(&board));
}

void print_board_code(Board board) {
    for (int i = 0; i < sizeof(board); i++) {
        printf("%02x", ((char *)&board)[i]);
    }
    printf("\n");
}