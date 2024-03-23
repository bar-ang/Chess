#include "diff.h"

void log_move(GameLog *log, Tile from, Tile to) {
    ASSERT(log->length < LOG_LEN);
    log->moves[log->length++] = {.from = from, .to = to};
}

GameLog new_log(Board *board) {
    return {.board = board, .length = 0};
}

void diff_of_boards(Diff *diff, Board *b1, Board *b2) {
    for (int i = 0; i < NUM_ROWS; i++)
        for (int j = 0; j < NUM_COLS; j++) {
            auto p1 = BOARD(*b1, i, j);
            auto p2 = BOARD(*b2, i, j);
            if (p1 == p2) continue;
            ASSERTE(p1 == NO_PIECE || p2 == NO_PIECE, printf("(%d, %d)\n", i, j));
            if (p1 != NO_PIECE)
                diff->from = {.row = i, .col = j};
            else
                diff->to = {.row = i, .col = j};
        }
}