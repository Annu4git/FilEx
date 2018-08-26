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
  
  ch = getchar(); /* standard getchar call */
  

  cout << ch << endl;



  if(ch == 27) {
    cout << "up key";
  } 
  if(ch == 66) {
    cout << "down key";
  }
  if(ch == 67) {
    cout << "right key";
  } 
  if(ch == 68) {
    cout << "left key";
  }
    return ch; /*return received char */
}