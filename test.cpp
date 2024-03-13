#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"

#define MSG_LEN 256
#define STOP(t, x, m) if(!(x)) { sprintf((t)->msg, m); return; }
#define HALT(t) if((t)->reult == TEST_FAILED) { return; }
#define RETURN_SUCCESS(t)  (t)->result = TEST_PASS; return; 
#define RETURN_FAIL(t, mesg) (t)->result = TEST_FAILED; sprintf(t->msg, mesg); return; 

enum testResults {
    TEST_NONE, TEST_PASS, TEST_FAILED
};

struct test_result {
    testResults result;
    char msg[MSG_LEN];
};

bool move_is_possible(Selection *select, int row, int col) {
    for (int i = 0; i < select->num_possible_moves; i++) {
        if (select->possible_moves[i].row == row && select->possible_moves[i].col == col)
            return true;
    }

    return false;
}


void test1(struct test_result *t) {
    /*
        "1 . . . . . . . ."
        "2 . . . . . . . ."
        "3 . . . . . . . ."
        "4 . . . R . . . ."
        "5 . . . . . . . ."
        "6 . . . . . . . ."
        "7 . . . . . . . ."
        "8 . . . . . . . ."
        "  A B C D E F G H"
    */
    auto board = init_board();
    board = set_piece(board, 0, PIECE_ROOK, PLAYER_WHITE, 3, 3);

    auto select = select_tile(&board, 3, 3);
    if (!move_is_possible(&select, 3, 4)) {
        RETURN_FAIL(t, "it should be possible to move the rook to (3, 4)")
    }
    if (!move_is_possible(&select, 4, 3)) {
        RETURN_FAIL(t, "it should be possible to move the rook to (4, 3)")
    }
    if (move_is_possible(&select, 4, 4)) {
        RETURN_FAIL(t, "it should NOT be possible to move the rook to (4, 4)")
    }
    
    RETURN_SUCCESS(t)
}

int show_results(struct test_result *t, int num_tests) {
    int failures = 0;
    for (int i = 0; i < num_tests; i++) {
        if (t[i].result == TEST_PASS) 
            printf("test %d PASSED!\n", i+1);
        else {
            printf("test %d FAILED: %s\n", i+1, t[i].msg);
            failures++;
        }
    }

    return failures;
}

int main() {
    printf("start testing\n");
    struct test_result results[10];

    test1(results);
    
    return show_results(results, 1);
}