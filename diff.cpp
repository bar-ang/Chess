#include "diff.h"

void log_move(GameLog *log, Tile from, Tile to) {
    log->moves[log->length++] = {.from = from, .to = to};
}

GameLog new_log(Board *board) {
    return {.board = board, .length = 0};
}