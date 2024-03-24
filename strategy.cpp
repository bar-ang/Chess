#include "strategy.h"

#define QUEUE_SIZE 1000

// Defining this type so that in the future 'bcode' will be made more memory-efficient than 'Board'
typedef Board bcode;

int calculate_best_next_step(Board *board, Player player, int depth, Board *best) {
    if (depth == 0) {
        if (best)
            *best = *board;
        return board_score(board);
    }
    Board buffer[NUM_TILES];
    int curr_best_score;
    int buffer_len;
    int best_score = (player == PLAYER_WHITE) ? -40 : 40;
    for (int i = 0; i < NUM_ROWS; i++)
        for (int j = 0; j < NUM_COLS; j++) {
            buffer_len = adjacent_boards(board, player, i, j, buffer);
            for (int k = 0; k < buffer_len; k++) {
                curr_best_score = calculate_best_next_step(&buffer[k], OPPONENT_OF(player), depth - 1, NULL);
                if ((player == PLAYER_WHITE && curr_best_score > best_score) || (player == PLAYER_BLACK && curr_best_score < best_score)) {
                    best_score = curr_best_score;
                    if (best)
                        *best = buffer[k];
                }
            }
        }
    return best_score;
}