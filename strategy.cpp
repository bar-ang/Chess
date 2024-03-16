#include "strategy.h"

#define QUEUE_SIZE 1000

// Defining this type so that in the future 'bcode' will be made more memory-efficient than 'Board'
typedef Board bcode;

typedef enum {
    QUEUE_OK, ERR_QUEUE_FULL, ERR_QUEUE_EMPTY
} queue_error;

typedef struct bounded_queue {
    int first_elem;
    int num_elements;
    bcode elems[QUEUE_SIZE];
} Queue;

void init_queue(Queue *q) {
    q->first_elem = 0;
    q->num_elements = 0;
}

queue_error insert(Queue *q, bcode *b, int num_elems) {
    if (q->num_elements + num_elems > QUEUE_SIZE)
        return ERR_QUEUE_FULL;

    for (int i = 0; i < num_elems; i++)
        q->elems[(q->first_elem + q->num_elements + i) % QUEUE_SIZE] = b[i];

    q->num_elements += num_elems;
    
    return QUEUE_OK;
}

queue_error remove(Queue *q) {
    if (q->num_elements == 0)
        return ERR_QUEUE_EMPTY;

    q->first_elem++;
    q->num_elements--;

    return QUEUE_OK;
}

queue_error inspect(bcode *res, Queue *q) {
    if (q->num_elements == 0)
        return ERR_QUEUE_EMPTY;
    *res = q->elems[q->first_elem];
    return QUEUE_OK;
}

int reach_out(Board *best, Board *board, Player player, int depth) {
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
                curr_best_score = reach_out(&curr_best, &buffer[k], OPPONENT_OF(player), depth - 1);
                if ((player == PLAYER_WHITE && curr_best_score > best_score) || (player == PLAYER_BLACK && curr_best_score < best_score)) {
                    best_score = curr_best_score;
                    *best = curr_best;
                }
            }
        }
    return best_score;
}