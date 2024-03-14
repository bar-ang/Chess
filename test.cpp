#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "printer.h"

#define MSG_LEN 256
#define STOP(t, x, m) if(!(x)) { sprintf((t)->msg, m); return; }
#define HALT(t) if((t)->reult == TEST_FAILED) { return; }
#define RETURN_SUCCESS(t)  (t)->result = TEST_PASS; return; 
#define RETURN_FAIL(t, mesg) (t)->result = TEST_FAILED; sprintf(t->msg, mesg); return; 
#define RETURN_FAIL_P(t, mesg, p1, p2) (t)->result = TEST_FAILED; sprintf(t->msg, mesg, p1, p2); return; 

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


void test_rook_move(struct test_result *t) {
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
    Board board;
    for (int x = 0; x < NUM_ROWS; x++)
        for (int y = 0; y < NUM_COLS; y++) {
            board = init_empty_board();
            board = set_piece(board, 0, PIECE_ROOK, PLAYER_WHITE, x, y);
            auto select = select_tile(&board, x, y);
        
            for (int i = 0; i < NUM_ROWS; i++)
                for (int j = 0; j < NUM_COLS; j++) {
                    if ((i == x || j == y) && !(i == x && j == y)) {
                        if (!move_is_possible(&select, i, j)) {
                            print_selection(select);
                            RETURN_FAIL_P(t, "it should be possible to move the rook to (%d, %d)", i, j)
                        }
                    } else if (move_is_possible(&select, i, j)) {
                        print_selection(select);
                        RETURN_FAIL_P(t, "it should NOT be possible to move the rook to (%d, %d)", i, j)
                    }
                            
                }
        }
    RETURN_SUCCESS(t)
}

void test_bishop_move(struct test_result *t) {
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
    Board board;
    for (int x = 0; x < NUM_ROWS; x++)
        for (int y = 0; y < NUM_COLS; y++) {
            board = init_empty_board();
            board = set_piece(board, 0, PIECE_BISHOP, PLAYER_BLACK, x, y);
            auto select = select_tile(&board, x, y);

            for (int i = 0; i < NUM_ROWS; i++)
                for (int j = 0; j < NUM_COLS; j++) {
                    if ((i + j == x + y || i - j == x - y) && !(i == x && j == y)) {
                        if (!move_is_possible(&select, i, j)) {
                            print_selection(select);
                            RETURN_FAIL_P(t, "it should be possible to move the bishop to (%d, %d)", i, j)
                        }
                    } else if (move_is_possible(&select, i, j)) {
                        print_selection(select);
                        RETURN_FAIL_P(t, "it should NOT be possible to move the bishop to (%d, %d)", i, j)
                    }

                }
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

int _main() {
    printf("start testing\n");
    struct test_result results[10];

    int c = 0;

    #define T &results[c++]
    
    test_rook_move(T);
    test_bishop_move(T);
    
    #undef T
    
    return show_results(results, c);
}