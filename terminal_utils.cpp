#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string>
#include <bits/stdc++.h>

#include "terminal_utils.h"

#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108

using namespace std;

/* variable to get default settings of keyboard */
struct termios default_settings;

/* variable of type termios to create custom settings */
struct termios custom_settings;

void keyboard_settings_off() {

	/* keep default settings to restore the keyboard settings */
	tcgetattr(STDIN_FILENO, &default_settings);

	/* initialize custom settings with default settings */
	custom_settings = default_settings;

	/* silent the ECHO */
	custom_settings.c_lflag &= ~(ICANON | ECHO);

	/* start the custome settings */
	tcsetattr(STDIN_FILENO, TCSANOW, &custom_settings);
}

void keyboard_settings_on() {

	/* restore keyboard default settings */
	tcsetattr(STDIN_FILENO, TCSANOW, &default_settings);
}

string keyboard_handle() {

	char ch;

	ch = getchar();

	string input;

	if(ch == 'A') {
		input = "UP";
	} else if(ch == 'B') {
		input = "DOWN";
	} else if(ch == 'C') {
		input = "RIGHT";
	} else if(ch == 'D') {
		input = "LEFT";
	} else if(ch == '\n') {
		input = "ENTER";
	} else if(ch == 'h' || ch == 'H') {
		input = "ROOT";
	} else if(ch == 127) {
		input = "BACKSLASH";
	} 

	return input;
}