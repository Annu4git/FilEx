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

void hold_terminal(terminal &app);

void clear_terminal();

void enter_into_directory(terminal &app, string directory_path, string mode, string search_query);

int trim_path(string &path, terminal app);

int trim_path_from_left(string &path, terminal app);

void debug(terminal &app, string debug_msg);

#endif