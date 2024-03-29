/*

Name  	: Anurag Chaturvedi
Roll No : 2018201024

*/

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string>
#include <bits/stdc++.h>

#include <sys/wait.h>

#include "terminal_utils.h"
#include "terminal.h"
#include "linux_cmd.h"
#include "command_mode.h"

#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108

using namespace std;

int trim_path(string &path, terminal app);



/* variable to get default settings of keyboard */
struct termios default_settings;

/* variable of type termios to create custom settings */
struct termios custom_settings;

void clear_terminal() {
	printf("\033c");
}

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

	int relative_index = 3;

	char ch;

	ch = getchar();

	string input;

	if(ch == ':') {
		input = "COLON";
	}
	else if(ch == 'A') {
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
		input = "BACKSPACE";
	} 

	return input;
}

void hold_terminal(terminal &app) {
	
	int relative_index = 3;
	
	string result_from_command_mode;

	while(1) {
		string input = keyboard_handle();

		if(input == "COLON") {
			result_from_command_mode = enter_in_command_mode(app);

			app.reset_cursor_position();

			input = "";

		}

		else if(input == "ROOT") {
			clear_terminal();

			string path = app.root_path;
			
			app.current_file_list = ls_impl(path, app);	// defined in linux_cmd

			app.reset_cursor_position();

		}
		else if(input == "UP") {

			int move_up = app.move_cursor_up();

			if(move_up == 1 && app.index_of_first_record_to_be_displayed != 0) {
				(app.index_of_first_record_to_be_displayed) -- ;

				clear_terminal();
				int local_x = app.cursor_position_x;
				int local_y = app.cursor_position_y;

				// app.reset_cursor_position();
				
				app.current_file_list = ls_impl(app.current_path, app);	// defined in linux_cmd

				app.set_cursor_position(local_x, local_y);

			}

		} else if(input == "DOWN") {

			int move_down = app.move_cursor_down();

			if(move_down == 1) {
				if((app.index_of_first_record_to_be_displayed + app.total_records_to_be_displayed) < app.total_records_in_current_directory) {
					(app.index_of_first_record_to_be_displayed) ++ ;

					clear_terminal();
					int local_x = app.cursor_position_x;
					int local_y = app.cursor_position_y;
					
					app.current_file_list = ls_impl(app.current_path, app);	// defined in linux_cmd
					//debug(app, "this is fine");
					app.set_cursor_position(local_x, local_y);
				}
			}

		} else if(input == "LEFT") {

			if(app.trace_pointer > 0) {

				app.decrement_trace_pointer();

				string path = app.trace[app.trace_pointer];

				app.current_file_list = ls_impl(path, app);	// defined in linux_cmd

				app.current_path = path;

			}
			
		} else if(input == "RIGHT") {

			if(app.trace[(app.trace_pointer)+1] != "0") {

				app.increment_trace_pointer();
				
				string path = app.trace[app.trace_pointer];
				
				app.current_file_list = ls_impl(path, app);	// defined in linux_cmd

				app.current_path = path;
			}
			
		} else if(input == "BACKSPACE") {

			int status;

			string path = app.current_path;

			path = path + "/..";

			trim_path(path, app);
			status = trim_path(path, app);
			
			if(status != 0) {

				app.current_file_list = ls_impl(path, app);	// defined in linux_cmd

				app.current_path = path;

				app.increment_trace_pointer();

				app.trace[app.trace_pointer] = path;
			}

			if(result_from_command_mode == "command-return-from-search") {
				enter_into_directory(app, app.current_path, "normal", "");
				result_from_command_mode = "";
			}
		}
		
		if(input == "ENTER") {

			enter_into_directory(app, "", "normal", "");

			input = "";
			
		}

	}
}

int trim_path(string &path, terminal app) {

	if(path.compare(app.root_path) == 0) {
		return 0;
	}

	int i;
	for(i = path.length() - 1; i >= 0; i--) {
		if(path[i] == '/') {
			break;
		}
	}

	path = path.substr(0, i);

	return 1;
}

int trim_path_from_left(string &path, terminal app) {

	int i;
	for(i = 0; i < path.length(); i++) {
		if(path[i] == '/') {
			break;
		}
	}

	path = path.substr(i+1, path.size() - i - 1);

	return 1;
}

vector < tuple < string, string, char > > enter_into_directory(terminal &app, string directory_path, string mode, string search_query) {

	int relative_index = 3;

	vector < tuple < string, string, char > > list;

	if(mode == "normal" && get<2>(app.current_file_list[app.cursor_position_x - relative_index]) == '-') {

		string path = get<1>(app.current_file_list[app.cursor_position_x - relative_index]);
		//system(("xdg-open " + path).c_str());	

		pid_t pid = fork();
		if (pid == 0) {
			execl("/usr/bin/xdg-open", "xdg-open", path.c_str(), (char *)0);
			exit(1);
			wait(NULL);
		}
		
		
	} else if(mode == "normal" && 
		get<0>(app.current_file_list[app.cursor_position_x - relative_index]) == ".") {

		string path = app.current_path;

		app.reset_index_of_first_record_to_be_displayed();

		app.current_file_list = ls_impl(path, app);	// defined in linux_cmd
		
	} else if(mode == "normal" && get<0>(app.current_file_list[app.cursor_position_x - relative_index]) == "..") {
		
		int status;

		string path = get<1>(app.current_file_list[app.cursor_position_x - relative_index]);

		trim_path(path, app);
		
		status = trim_path(path, app);
		
		if(status != 0) {

			app.reset_index_of_first_record_to_be_displayed();

			app.current_file_list = ls_impl(path, app);	// defined in linux_cmd

			app.current_path = path;

			app.increment_trace_pointer();

			app.trace[app.trace_pointer] = path;

			app.increment_trace_pointer();

			app.trace[app.trace_pointer] = "0";

			app.decrement_trace_pointer();

		}
		
	} else {
		
		app.reset_index_of_first_record_to_be_displayed();
		
		string path;

		if(mode == "command-goto") {
			path = directory_path;
			app.current_file_list = ls_impl(path, app);	// defined in linux_cmd
			mode = "normal";

		} else if (mode == "command-search") {
			path = directory_path;
			vector < tuple < string, string, char > > searched_list 
			= ls_impl_with_search(path, app, search_query);	// defined in linux_cmd

			if(searched_list.size() == 0) {
				string path = app.current_path;

				app.reset_index_of_first_record_to_be_displayed();

				app.current_file_list = ls_impl(path, app);	// defined in linux_cmd

			}

			return searched_list;

		} else if(mode == "command-return-from-search") {

			path = directory_path;
			app.current_file_list = ls_impl(path, app);	// defined in linux_cmd
			mode = "normal";

		} else {
			path = get<1>(app.current_file_list[app.cursor_position_x - relative_index]);
			app.current_file_list = ls_impl(path, app);	// defined in linux_cmd

		}
		
		
		if(mode != "command-search") {
			app.current_path = path;

			app.increment_trace_pointer();

			app.trace[app.trace_pointer] = path;

			app.increment_trace_pointer();

			app.trace[app.trace_pointer] = "0";

			app.decrement_trace_pointer();
		}
	}

	return list;
}

void debug(terminal &app, string debug_msg) {
	int local_x = app.cursor_position_x;
	int local_y = app.cursor_position_y;

	string print_space = "";
	app.set_cursor_position(app.message_row_no, 1, true);
	for(int i = 0; i < app.terminal_col; i++) {
		print_space = print_space + " ";
	}
	app.print_text(print_space);

	app.set_cursor_position(app.message_row_no, 1, true);
	app.print_text(debug_msg);
	getchar();
	app.set_cursor_position(local_x, local_y);
}