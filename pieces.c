#include "pieces.h"

extern int board[8][8];
extern bool movements[8][8];

void get_rook_moves(Player player, int y, int x)
{
  int a = y, b = x;

  while (--y >= 0 && !PIECE_OF(player, y, x))
  {
    movements[y][x] = true;
    if (PIECE_OF(!player, y, x))
      break;
  }
  y = a;
  while (++x <= 7 && !PIECE_OF(player, y, x))
  {
    movements[y][x] = true;
    if (PIECE_OF(!player, y, x))
      break;
  }
  x = b;
  while (++y <= 7 && !PIECE_OF(player, y, x))
  {
    movements[y][x] = true;
    if (PIECE_OF(!player, y, x))
      break;
  }
  y = a;
  while (--x >= 0 && !PIECE_OF(player, y, x))
  {
    movements[y][x] = true;
    if (PIECE_OF(!player, y, x))
      break;
  }
}

void get_bishop_moves(Player player, int y, int x)
{
  int a = y, b = x;
  
  while ((--y >= 0 && ++x <= 7) && !PIECE_OF(player, y, x))
  {
    movements[y][x] = true;
    if (PIECE_OF(!player, y, x))
      break;
  }
  y = a, x = b;
  while ((++y <= 7 && ++x <= 7) && !PIECE_OF(player, y, x))
  {
    movements[y][x] = true;
    if (PIECE_OF(!player, y, x))
      break;
  }
  y = a, x = b;
  while ((++y <= 7 && --x >= 0) && !PIECE_OF(player, y, x))
  {
    movements[y][x] = true;
    if (PIECE_OF(!player, y, x))
      break;
  }
  y = a, x = b;
  while ((--y >= 0 && --x >= 0) && !PIECE_OF(player, y, x))
  {
    movements[y][x] = true;
    if (PIECE_OF(!player, y, x))
      break;
  }
} 

void get_knight_moves(Player player, int y, int x)
{
  static int comb[8][2] = {{-2,1},{-1,2},{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1}};
  int p_y, p_x;

  for (int i = 0; i < 8; i++)
  {
     p_y = y + comb[i][0];
     p_x = x + comb[i][1];
    if (INSIDE_BOARD(p_y, p_x) && !PIECE_OF(player, p_y, p_x))
      movements[p_y][p_x] = true;
  }
}

void get_queen_moves(Player player, int y, int x)
{
  get_rook_moves(player, y, x);
  get_bishop_moves(player, y, x);
}

void get_king_moves(Player player, int y, int x)
{
  static int comb[8][2] = {{-1,0},{-1,1},{0,1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}};

  for (int i = 0; i < 8; i++)
  {
    int p_y = y + comb[i][0];
    int p_x = x + comb[i][1];
    if (INSIDE_BOARD(p_y, p_x) && !PIECE_OF(player, p_y, p_x))
      movements[p_y][p_x] = true;
  }
}
 
void get_pawn_moves(Player player, int y, int x)
{
  int f = player ? -1 : 1;
  
  if (INSIDE_BOARD(y+f, x) && board[y+f][x] == 0)
    movements[y+f][x] = true;
  if (INSIDE_BOARD(y+f, x+1) && PIECE_OF(!player, y+f, x+1))
    movements[y+f][x+1] = true;
  if (INSIDE_BOARD(y+f, x-1) && PIECE_OF(!player, y+f, x-1))
    movements[y+f][x-1] = true;
  if (INSIDE_BOARD(y+2*f, x) && (player ? y == 6 : y == 1))
    movements[y+2*f][x] = true;
}
