#include "move.h"

#define EMPTY ' '

#define NO_COLOR            0
#define COLOR_RED           91
#define COLOR_YELLOW        92
#define COLOR_BLUE          94
#define COLOR_BG_CYAN       46
#define COLOR_BG_MAGENTA    45
#define COLOR_BG_GRAY       100
#define COLOR_BG_BLACK      40
#define COLOR_BG_YELLOW     43
#define COLOR_BG_RED        41
#define COLOR_WHITE         97
#define COLOR_BG_WHITE      107
#define COLOR_BLACK         40


#define WHITE_TILE_BGCOLOR         COLOR_BG_GRAY
#define BLACK_TILE_BGCOLOR         COLOR_BG_BLACK
#define WHITE_PLAYER_COLOR         COLOR_BLUE
#define BLACK_PLAYER_COLOR         COLOR_RED
#define SELECTED_PIECE_BGCOLOR     COLOR_BG_YELLOW
#define POSSIBLE_MOVE_BGCOLOR      COLOR_BG_CYAN
#define THREATENED_PIECES_BGCOLOR  COLOR_BG_MAGENTA
#define EDGE_COLOR                 COLOR_BLACK
#define EDGE_BGCOLOR               COLOR_BG_WHITE

#define PRINTCOLOR(fg, bg) printf("\033[1;%d;%dm", (fg), (bg))
#define RESETCOLOR PRINTCOLOR(NO_COLOR, NO_COLOR)

bool sign(char *, PieceType);
void print_board(Board);
void print_selection(Selection);
void print_board_code(Board);
bool from_string(Tile *tile, char *str);
bool to_string(char *str, Tile tile);
Tile user_input();