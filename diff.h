#include "move.h"

#define LOG_LEN 1000

typedef struct {
    Tile from;
    Tile to;
} Diff;

typedef struct {
    Board *board;
    Diff moves[LOG_LEN];
    int length;
} GameLog;

void log_move(GameLog *, Tile, Tile);
GameLog new_log(Board *);