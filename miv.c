#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef linux
#include <unistd.h>
#endif
#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif


#define CTRL(x) ((x) & 0x1f)

#define BUFFSIZE (2048 * 2048)

unsigned int
newlsBefore(char *t, unsigned int pos)
{
  unsigned int c = 0;
  for (unsigned int i = 0; i < pos; ++i)
    if (t[i] == '\n')
      ++c;
  return c - 1;
}

unsigned int
lenPrevLines(char *t, unsigned int pos)
{
  unsigned int c = pos;
  while (t[c] != '\n')
    --c;
  return c + 1;
}

void
writeFile(char *text, char *path)
{
  FILE *fp = fopen(path, "w");
  char* temp = malloc(BUFFSIZE);
  memcpy(temp, text, BUFFSIZE);
  for(unsigned int i = 0; i < BUFFSIZE - 1; i++)
    temp[i] = temp[i + 1];
  temp[strlen(temp)] = '\n';
  fprintf(fp, "%s", temp);
  fclose(fp);
  free(temp);
}

int
main(int argc, char **argv)
{
  char *text = malloc(BUFFSIZE);
  if (access(argv[1], F_OK) == 0) {
    FILE *fp = fopen(argv[1], "r");
    fread(text + 1, BUFFSIZE - 1, 1, fp);
    fclose(fp);
  }
  unsigned int pos = 1;
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  int run = 1;
  text[0] = '\n';
  while (run) {
    /* RENDER */
    clear();
    printw("%s", text);
    move(newlsBefore(text, pos) + 1, pos - lenPrevLines(text, pos));
    refresh();
    /** INPUT */
    int ch = 0;
    ch = getch();
    switch (ch) {
    case KEY_LEFT:
      --pos;
      break;
    case KEY_RIGHT:
      ++pos;
      break;
    case KEY_BACKSPACE:
      text[pos - 1] = 0;
      --pos;
      break;
    case KEY_ENTER:
      text[pos] = '\n';
      break;
    case CTRL('c'):
      run = 0;
      break;
    case CTRL('w'):
      writeFile(text, argv[1]);
      break;
    default:
      text[pos] = ch;
      ++pos;
      break;
    }
  }
  getch();
  endwin();
  printf("\n");
  free(text);

  return 0;
}
