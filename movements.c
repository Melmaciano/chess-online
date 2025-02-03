#include "movements.h"

extern void print_screen(void);

extern int board[8][8];
bool movements[8][8] = {0};


void reset_movements(void)
{
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      movements[i][j] = false;
}

void color_movements(void)
{
  attrset(COLOR_PAIR(1));

  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++)
      if (movements[y][x])
        mvaddch(y + 1, x * 2 + 1, (board[y][x] ? board[y][x] : '.'));

  refresh();
  attrset(A_NORMAL);
}

void get_moves(Player player, int y, int x)
{
  reset_movements();
  switch(board[y][x])
  {
    case 'p': case 'P':
      get_pawn_moves(player, y, x); break;
    case 'r': case 'R':
      get_rook_moves(player, y, x); break;
    case 'c': case 'C':
      get_knight_moves(player, y, x); break;
    case 'b': case 'B':
      get_bishop_moves(player, y, x); break;
    case 'q': case 'Q':
      get_queen_moves(player, y, x); break;
    case 'k': case 'K':
      get_king_moves(player, y, x); break;
  }
  color_movements();
}

void play_turn(Player player, int *coords)
{
  int ch, cur_y, cur_x, new_y, new_x;
  MEVENT mouse;

  select_new_piece:
  while ((ch = getch()) != KEY_MOUSE);
  getmouse(&mouse);
  cur_y = mouse.y - 1;
  cur_x = (mouse.x - 1) / 2;
  if (!INSIDE_BOARD(cur_y, cur_x) || 
      !PIECE_OF(player, cur_y, cur_x))
    goto select_new_piece;

  get_possible_moves:
  get_moves(player, cur_y, cur_x);

  select_move:
  while ((ch = getch()) != KEY_MOUSE);
  getmouse(&mouse);
  new_y = mouse.y - 1;
  new_x = (mouse.x - 1) / 2;

  if (!INSIDE_BOARD(cur_y, cur_x))
    goto select_move;
  else if (PIECE_OF(player, new_y, new_x))
  {
    print_screen();
    cur_y = new_y;
    cur_x = new_x;
    goto get_possible_moves;
  }
  else if (!movements[new_y][new_x])
    goto select_move;

  coords[0] = cur_y;
  coords[1] = cur_x;
  coords[2] = new_y;
  coords[3] = new_x;

  flushinp();
}
