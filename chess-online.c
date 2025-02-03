#include "movements.h"
#include "network.h"

#define SERVER_PORT 8080

void print_screen(void);
int update_board(int *coords);
void check_gameover(int ch, int s1, int s2);

int board[8][8] = {
  [0] = {'r','c','b','q','k','b','c','r'},
  [1] = {'p','p','p','p','p','p','p','p'},
  [6] = {'P','P','P','P','P','P','P','P'},
  [7] = {'R','C','B','Q','K','B','C','R'}
};

int main(int argc, char *argv[])
{
  Player player = (argc == 1 ? WHITE : BLACK);
  int claimed_piece;
  int coords[4];
  
  // screen configuration
  initscr();
  noecho();
  keypad(stdscr,TRUE);
  curs_set(0);

  if (NCURSES_MOUSE_VERSION > 0)
    mousemask(BUTTON1_RELEASED | BUTTON1_CLICKED, NULL);
  if (has_colors() && start_color() == OK)
    init_pair(1, COLOR_WHITE, COLOR_RED);

  // network configuration
  if (player == BLACK)
  {
    struct hostent *hp;
    struct sockaddr_in sin;
    char *host;
    int socket_peer;

    if (argc == 2)
    {
      host = argv[1];
    }
    else
    {
      fprintf(stderr, "usage: chess_online host\n");
      endwin();
      exit(1);
    }

    // translate host name into peer's IP address
    hp = gethostbyname(host);
    if (!hp) {
      fprintf(stderr, "chess_online: unknown host: %s\n", host);
      endwin();
      exit(1);
    }

    // build address data structure 
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons(SERVER_PORT);

    // active open
    if ((socket_peer = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror("chess_online: socket");
      endwin();
      exit(1);
    }
    if (connect(socket_peer, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
      perror("chess_online: connect");
      close(socket_peer);
      endwin();
      exit(1);
    }
    while (true)
    {
      print_screen();
      // recieve
      recv(socket_peer, coords, sizeof(coords), 0);
      claimed_piece = update_board(coords);
      check_gameover(claimed_piece, socket_peer, -1);

      print_screen();
      // send
      play_turn(player, coords);
      send(socket_peer, coords, sizeof(coords), 0);
      claimed_piece = update_board(coords);
      check_gameover(claimed_piece, socket_peer, -1);
    }
  }
  else
  {
    struct sockaddr_in sin;
    socklen_t addr_len;
    int socket_listen, socket_peer;

    // build address data structure
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

    // setup passive open
    if ((socket_listen = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror("chess_online: socket");
      endwin();
      exit(1);
    }
    if ((bind(socket_listen, (struct sockaddr *)&sin, 
          sizeof(sin))) < 0) 
    {
      perror("chess_online: bind");
      endwin();
      close(socket_listen);
      exit(1);
    }
    listen(socket_listen, 0);

    // establish peer socket
    if ((socket_peer = accept(socket_listen,
          (struct sockaddr *)&sin, &addr_len)) < 0) 
    {
      perror("chess_online: accept");
      endwin();
      close(socket_listen);
      exit(1);
    }

    while(true)
    {
      print_screen();
      // send
      play_turn(player, coords);
      send(socket_peer, coords, sizeof(coords), 0);
      claimed_piece = update_board(coords);
      check_gameover(claimed_piece, socket_peer, -1);

      print_screen();
      // recieve
      recv(socket_peer, coords, sizeof(coords), 0);
      claimed_piece = update_board(coords);
      check_gameover(claimed_piece, socket_peer, -1);
    }
  }
}

void check_gameover(int claimed_piece, int s1, int s2)
{
  if (claimed_piece == 'k' || claimed_piece == 'K')
  {
    close(s1);
    if (s2 >= 0)
      close(s2);
    endwin();
    printf("%s won\n", claimed_piece == 'k' ? "white" : "black");
    exit(0);
  }
}


int update_board(int *coords)
{
  int ch, cur_y, cur_x, new_y, new_x;

  cur_y = coords[0];
  cur_x = coords[1];   
  new_y = coords[2];   
  new_x = coords[3];   
  ch = board[new_y][new_x];

  board[new_y][new_x] = board[cur_y][cur_x];
  board[cur_y][cur_x] = 0;

  if (board[new_y][new_x] == 'p' && new_y == 7 )
    board[new_y][new_x] = 'q';
  else if (board[new_y][new_x] == 'P' && new_y == 0)
    board[new_y][new_x] = 'Q';

  return ch;
}

void print_screen(void)
{
  erase(); 
  mvaddch(0,0,' ');
  for (int i_x = 'A'; i_x < 'A' + 8; i_x++)
  {
    addch(i_x);
    addch(' ');
  }

  for (int i_y = '0', i = 1; i_y < '0' + 8; i_y++, i++)
    mvaddch(i, 0, i_y);

  for (int y = 0; y < 8; y++)
  {
    move(y+1,1);
    for (int x = 0; x < 8; x++)
    { 
      if (board[y][x] == 0)
      addch('.');
      else
        addch(board[y][x]);
      addch(' ');
    }
  }
  refresh();
}
