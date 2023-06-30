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
#define BUFFSIZE (1024 * 1024)

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

unsigned int
lenUntilNextLn(char *text, unsigned int pos)
{
  unsigned int c = 0;
  while (text[pos + c] != '\n')
    ++c;
  return c;
}

unsigned int
lenUntilPrevLn(char *text, unsigned int pos)
{
  unsigned int c = 0;
  while (text[pos - c] != '\n')
    ++c;
  return c;
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

void
insertAt(char *text, char c, unsigned int pos)
{
  unsigned int i;
  char temp = text[pos];
  for(i = BUFFSIZE - 1; i >= pos + 1; --i)
  {
    text[i + 1] = text[i];
  }
  text[i] = c;
  text[i + 1] = temp;
}

void
removeAt(char *text, unsigned int pos)
{
  unsigned int i;
  /* Copy next element value to current element */
  for(i = pos; i < BUFFSIZE - 1; ++i)
  {
    text[i] = text[i + 1];
  }
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
  /* scrollok(stdscr, TRUE); */
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
      if (pos != 0)
        --pos;
      break;
    case KEY_RIGHT:
      ++pos;
      break;
    case KEY_DOWN:
      pos += lenUntilNextLn(text, pos) + lenUntilPrevLn(text, pos);
      break;
    case KEY_UP:
      text[strlen(text)] = '\n';
      pos -= lenUntilNextLn(text, pos) + lenUntilPrevLn(text, pos);
      text[strlen(text) - 1] = 0;
      break;
    case KEY_BACKSPACE:
      if (pos != 1) {
        removeAt(text, pos - 1);
        --pos;
      }
      break;
    case KEY_ENTER:
      insertAt(text, '\n', pos);
      break;
    case CTRL('c'):
      run = 0;
      break;
    case CTRL('w'):
      writeFile(text, argv[1]);
      break;
    default:
      insertAt(text, ch, pos);
      ++pos;
      break;
    }
  }
  getch();
  endwin();
  free(text);

  return 0;
}
