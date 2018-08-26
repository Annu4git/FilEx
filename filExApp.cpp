#include <iostream>
#include <string>
#include <bits/stdc++.h>

#define clear_terminal() printf("\033[H\033[J")

#include "linux_cmd.h"
#include "terminal_utils.h"

using namespace std;

int main() {

	int cursor_position_x = 1, cursor_position_y = 1;

	keyboard_settings_off();

	clear_terminal();

	vector<string> file_list = ls_impl();	// defined in linux_cmd
	
	cursor_position(cursor_position_x, cursor_position_y);
	
	while(1) {
		string input = keyboard_handle();
		if(input == "UP") {
			cursor_position(--cursor_position_x, cursor_position_y);
		} else if(input == "DOWN") {
			cursor_position(++cursor_position_x, cursor_position_y);
		} else if(input == "RIGHT") {
			cursor_position(cursor_position_x, ++cursor_position_y);
		} else if(input == "LEFT") {
			cursor_position(cursor_position_x, --cursor_position_y);
		}
		
		if(input == "ENTER") {
			clear_terminal();

			string path = file_list[cursor_position_x-1];
			file_list = ls_impl(path);	// defined in linux_cmd

			cursor_position_x = 1;
			cursor_position_y = 1;

			cursor_position(cursor_position_x, cursor_position_y);
			
		}

	}
	fflush(stdin);

	keyboard_settings_on();
	return 0;	
}
