#include "board.h"

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