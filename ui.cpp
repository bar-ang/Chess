#include <string.h>
#include "ui.h"

bool from_string(Tile *tile, char *str) {
    if (strlen(str) < 2)
        return false;

    if (str[0] < '1' || str[0] > '8' || str[1] < 'A' || str[1] > 'H')
        return false;
    
    tile->row = str[0] - '1';
    tile->col = str[1] - 'A';

    return true;
}

bool to_string(char *str, Tile tile) {
    if (tile.row < 0 || tile.row > 7 || tile.col < 0 || tile.col > 7)
        return false;

    sprintf(str, "%c%d", tile.row + '1', tile.col + 'A');
    return true;
}

Tile user_input() {
    char input[3];
    Tile tile;
    if (scanf("%s", input) <= 0) {
        printf("Bad thing just happened... =[\n");
        return NO_TILE;
    }

    if (!from_string(&tile, input))
        return NO_TILE;

    return tile;
}
