#include <iostream>
#include "printer.h"

int main() {
    Board board = init_board();
    board.selected_piece = get_piece(board, 6, 1);
    print_board(board);

    return 0;
}