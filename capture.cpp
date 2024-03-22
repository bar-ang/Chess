#include <stdio.h>
#include <string.h>
#include "capture.h"


Capture new_capture(Board *initial_board) {
    Capture cap;

    cap.board = *initial_board;
    cap.num_adjs = 0;
    cap.flag = CAPTURE_NORMAL;

    return cap;
}

Capture *find_capture(Board *initial_board, Capture **list, int list_size) {
    for (int i = 0; i < list_size; i++) {
        if (memcmp(&list[i]->board, initial_board, sizeof(*initial_board)) == 0)
            return list[i];
    }

    return NULL;
}

bool add_adjacent(Capture *capture, Board *board, int num_boards, Capture **list, int *list_size) {
    ASSERT(num_boards > 0)
    if (num_boards == 0)
        return true;
    if (capture->num_adjs == 0) 
        capture->adj = (Capture **)malloc(sizeof(Capture *) * num_boards);
    else
        capture->adj = (Capture **)realloc(capture->adj, sizeof(Capture *) * (num_boards + capture->num_adjs));

    if (!capture->adj)
        return false;

    for (int i = 0; i < num_boards; i++) {
        Capture *cap;
        cap = find_capture(&board[i], list, *list_size);
        if (!cap) {
            cap = new_capture(&board[i]);
            list[(*list_size)++] = cap;
        }
        capture->adj[capture->num_adjs + i] = cap;
    }

    return true;
}

void free_capture(Capture *capture) {
    free(capture->adj);
    capture->num_adjs = 0;
}