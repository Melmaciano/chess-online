#include <stdbool.h>
#include <ctype.h>
#include <ncurses.h>

#define PIECE_OF(player, y, x) \
        ((player) ? \
        isupper(board[(y)][(x)]) : \
        islower(board[(y)][(x)]))
#define INSIDE_BOARD(y, x) ((y) >= 0 && (y) <= 7 && (x) >= 0 && (x) <= 7)

typedef bool Player;
enum players { BLACK = 0, WHITE = 1 };

void get_rook_moves(Player player, int y, int x);
void get_bishop_moves(Player player, int y, int x);
void get_knight_moves(Player player, int y, int x);
void get_queen_moves(Player player, int y, int x);
void get_king_moves(Player player, int y, int x);
void get_pawn_moves(Player player, int y, int x);
