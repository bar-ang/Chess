#include "move.h"

#define MAX_NUM_CAPTURES 1000

enum capture_state {
    CAPTURE_NORMAL, CAPTURE_SEEN
};

typedef struct cap_t {
        Board board;
        struct cap_t **adj;
        int num_adjs;
        enum capture_state flag;
} Capture;


Capture      new_capture();
bool         add_adjacent(Capture *, Board *, int);
void         free_capture(Capture *);