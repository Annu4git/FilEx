#include <iostream>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>

#define clear_terminal() printf("\033[H\033[2J")

#include "linux_cmd.h"
#include "terminal_utils.h"
#include "terminal.h"

using namespace std;

terminal app;

void hold_terminal(vector < tuple < string, string, char > > file_list);

int trim_path(string &path);

int relative_index = 3;

int main() {

	keyboard_settings_off();

	clear_terminal();

	app.reset_cursor_position();

	vector < tuple < string, string, char > >  file_list = ls_impl(true, app);	// defined in linux_cmd
	
	app.current_path = app.root_path;

	app.set_cursor_position();

	app.increment_trace_pointer();
	app.trace[app.trace_pointer] = app.root_path;
	
	hold_terminal(file_list);

	fflush(stdin);

	keyboard_settings_on();
	return 0;	
}

void hold_terminal(vector < tuple < string, string, char > > file_list) {
	while(1) {
		string input = keyboard_handle();
		if(input == "ROOT") {
			clear_terminal();

			string path = app.root_path;
			
			file_list = ls_impl(path, app);	// defined in linux_cmd

			cout << endl << "Printing root path should be same : " << app.root_path << endl;

			app.reset_cursor_position();

		}
		else if(input == "UP") {

			int move_up = app.move_cursor_up();

			if(move_up == 1 && app.index_of_first_record_to_be_displayed != 0) {
				(app.index_of_first_record_to_be_displayed) -- ;

				clear_terminal();
				int local_x = app.cursor_position_x;
				int local_y = app.cursor_position_y;

				app.reset_cursor_position();
				
				file_list = ls_impl(app.current_path, app);	// defined in linux_cmd

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

					app.reset_cursor_position();
					
					file_list = ls_impl(app.current_path, app);	// defined in linux_cmd

					app.set_cursor_position(local_x, local_y);
				}
			}

		} else if(input == "LEFT") {

			if(app.trace_pointer != -1) {
				clear_terminal();

				string path = app.trace[app.trace_pointer];
				app.decrement_trace_pointer();
				
				file_list = ls_impl(path, app);	// defined in linux_cmd

				app.reset_cursor_position();

				app.current_path = path;
			}
			
			
		} else if(input == "RIGHT") {

			if(app.trace[(app.trace_pointer)+1] != "0") {
				clear_terminal();
				app.increment_trace_pointer();
				string path = app.trace[app.trace_pointer];
				
				file_list = ls_impl(path, app);	// defined in linux_cmd

				app.reset_cursor_position();

				app.current_path = path;
			}
			
			
		} else if(input == "BACKSLASH") {

			int status;

			string path = app.current_path;

			path = path + "/..";

			trim_path(path);
			status = trim_path(path);
			
			if(status != 0) {

				clear_terminal();
				
				file_list = ls_impl(path, app);	// defined in linux_cmd

				cout << endl << "Printing root path should be same : " << app.root_path << endl;

				app.reset_cursor_position();

				app.current_path = path;

				app.increment_trace_pointer();

				app.trace[app.trace_pointer] = path;
			}
		}
		
		if(input == "ENTER") {

			/*int pid = fork();
			if (pid == 0) {
  				execl("/usr/bin/xdg-open", "xdg-open", file_list[app.cursor_position_x - relative_index], (char *)0);
  				exit(1);
			}*/	

			if(get<2>(file_list[app.cursor_position_x - relative_index]) == '-') {
				system(("xdg-open " + get<1>(file_list[app.cursor_position_x - relative_index])).c_str());	
				string path = get<1>(file_list[app.cursor_position_x - relative_index]);
				printf("\033[32;1H");
				cout << endl << "hell yeah is : " << path << endl;
				printf("\033[1;1H");
			} else if(get<0>(file_list[app.cursor_position_x - relative_index]) == ".") {

				string path = get<1>(file_list[app.cursor_position_x - relative_index]);
				trim_path(path);
				printf("\033[32;1H");
				cout << endl << "                                                    ";
				cout << "                                                ";
				printf("\033[32;1H");
				cout << endl << "single dot hell yeah is : " << path << endl;
				printf("\033[1;1H");
			} else if(get<0>(file_list[app.cursor_position_x - relative_index]) == "..") {
				
				int status;

				string path = get<1>(file_list[app.cursor_position_x - relative_index]);

				trim_path(path);
				status = trim_path(path);
				
				if(status != 0) {
					clear_terminal();

					file_list = ls_impl(path, app);	// defined in linux_cmd

					app.reset_cursor_position();

					app.current_path = path;

					app.increment_trace_pointer();

					app.trace[app.trace_pointer] = path;
				}
				
			} else {
				
				app.reset_index_of_first_record_to_be_displayed();
				
				string path = get<1>(file_list[app.cursor_position_x - relative_index]);

				clear_terminal();

				printf("\033[32;1H");
				cout << endl << "path is : " << path << endl;
				app.reset_cursor_position();
				
				
				file_list = ls_impl(path, app);	// defined in linux_cmd

				app.current_path = path;

				app.increment_trace_pointer();

				app.trace[app.trace_pointer] = path;

				app.increment_trace_pointer();

				app.trace[app.trace_pointer] = "0";

				app.decrement_trace_pointer();

			}
			
		}

	}
}

int trim_path(string &path) {

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