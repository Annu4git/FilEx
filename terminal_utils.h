#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string>

#include "terminal.h"

using namespace std;

#ifndef INC_TERMINAL_UTILS_H
#define INC_TERMINAL_UTILS_H

void keyboard_settings_off();

void keyboard_settings_on();

string keyboard_handle();

void hold_terminal(vector < tuple < string, string, char > > file_list, terminal &app);

void clear_terminal();

#endif