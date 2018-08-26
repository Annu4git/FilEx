#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>

#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108

using namespace std;

int getch(void);

int main()
{
 char x = ' ';
 x = getch();

 //cout << x;
 return 0;
}

int getch(void)
{
  char ch;
  struct termios oldt;
  struct termios newt;
  tcgetattr(STDIN_FILENO, &oldt); /*store old settings */
  newt = oldt; /* copy old settings to new settings */
  newt.c_lflag &= ~(ICANON | ECHO); /* make one change to old settings in new settings */
  tcsetattr(STDIN_FILENO, TCSANOW, &newt); /*apply the new settings immediatly */
  ch = getchar(); /* standard getchar call */
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /*reapply the old settings */

  cout << ch << endl;
  if(ch == KEY_UP) {
    cout << "up key";
  } 
  if(ch == KEY_DOWN) {
    cout << "down key";
  }
  if(ch == KEY_RIGHT) {
    cout << "right key";
  } 
  if(ch == KEY_LEFT) {
    cout << "left key";
  }
    return ch; /*return received char */
}