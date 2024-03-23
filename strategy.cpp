#include "strategy.h"

#define QUEUE_SIZE 1000

// Defining this type so that in the future 'bcode' will be made more memory-efficient than 'Board'
typedef Board bcode;

int calculate_best_next_step(Board *best, Board *board, Player player, int depth) {
    if (depth == 0) {
        *best = *board;
        return board_score(board);
    }
    Board buffer[NUM_TILES];
    Board curr_best;
    int curr_best_score;
    int buffer_len;
    int best_score = (player == PLAYER_WHITE) ? -40 : 40;
    for (int i = 0; i < NUM_ROWS; i++)
        for (int j = 0; j < NUM_COLS; j++) {
            buffer_len = adjacent_boards(board, player, i, j, buffer);
            for (int k = 0; k < buffer_len; k++) {
                curr_best_score = calculate_best_next_step(&curr_best, &buffer[k], OPPONENT_OF(player), depth - 1);
                if ((player == PLAYER_WHITE && curr_best_score > best_score) || (player == PLAYER_BLACK && curr_best_score < best_score)) {
                    best_score = curr_best_score;
                    *best = curr_best;
                }
            }
        }
    return best_score;
}