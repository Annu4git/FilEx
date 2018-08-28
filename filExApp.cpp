#include <iostream>
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

int main() {

	app.reset_cursor_position();

	keyboard_settings_off();

	clear_terminal();

	vector < tuple < string, string, char > >  file_list = ls_impl(true, app.root_path);	// defined in linux_cmd
	app.trace[++app.trace_pointer] = app.root_path;
	app.current_path = app.root_path;

	cout << endl << "Printing root path only for first time : " << app.root_path << endl;
	app.set_cursor_position();
	
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
			printf("\033[32;1H");
			cout << endl << "path is : " << path << endl;
			printf("\033[1;1H");
			file_list = ls_impl(path);	// defined in linux_cmd

			cout << endl << "Printing root path should be same : " << app.root_path << endl;

			app.reset_cursor_position();

		}
		else if(input == "UP") {

			app.move_cursor_up();

		} else if(input == "DOWN") {

			app.move_cursor_down();

		} else if(input == "RIGHT") {

			app.move_cursor_right();
			
		} else if(input == "LEFT") {

			app.move_cursor_left();
			
		} else if(input == "BACKSLASH") {

			int status;

			string path = app.current_path;

			path = path + "/..";

			trim_path(path);
			status = trim_path(path);
			
			if(status != 0) {

				clear_terminal();
				printf("\033[32;1H");

				cout << endl << "Trim path is : " << path << endl;
				printf("\033[1;1H");
				file_list = ls_impl(path);	// defined in linux_cmd

				cout << endl << "Printing root path should be same : " << app.root_path << endl;

				app.cursor_position_x = 1;
				app.cursor_position_y = 1;

				app.set_cursor_position();

				app.current_path = path;
			}
		}
		
		if(input == "ENTER") {

			/*int pid = fork();
			if (pid == 0) {
  				execl("/usr/bin/xdg-open", "xdg-open", file_list[app.cursor_position_x-1], (char *)0);
  				exit(1);
			}*/	
			cout << " ";

			if(get<2>(file_list[app.cursor_position_x-1]) == '-') {
				system(("xdg-open " + get<1>(file_list[app.cursor_position_x-1])).c_str());	
				string path = get<1>(file_list[app.cursor_position_x-1]);
				printf("\033[32;1H");
				cout << endl << "hell yeah is : " << path << endl;
				printf("\033[1;1H");
			} else if(get<0>(file_list[app.cursor_position_x-1]) == ".") {

				string path = get<1>(file_list[app.cursor_position_x-1]);
				trim_path(path);
				printf("\033[32;1H");
				cout << endl << "                                                    ";
				cout << "                                                ";
				printf("\033[32;1H");
				cout << endl << "single dot hell yeah is : " << path << endl;
				printf("\033[1;1H");
			} else if(get<0>(file_list[app.cursor_position_x-1]) == "..") {
				
				int status;

				string path = get<1>(file_list[app.cursor_position_x-1]);

				trim_path(path);
				status = trim_path(path);
				
				if(status != 0) {
					clear_terminal();

					printf("\033[32;1H");
					cout << endl << "                                                    ";
					cout << "                                                ";
					cout << endl << "Trim path is : " << path << endl;
					printf("\033[1;1H");
					file_list = ls_impl(path);	// defined in linux_cmd

					app.trace[++app.trace_pointer] = path;

					app.reset_cursor_position();

					app.current_path = path;
				}
				
			} else {
				clear_terminal();

				printf("\033[31;1H");
				cout << endl << "break2 " << endl;
				printf("\033[1;1H");

				string path = get<1>(file_list[app.cursor_position_x-1]);
				printf("\033[32;1H");
				cout << endl << "path is : " << path << endl;
				printf("\033[1;1H");
				file_list = ls_impl(path);	// defined in linux_cmd

				app.cursor_position_x = 1;
				app.cursor_position_y = 1;

				app.set_cursor_position();

				app.current_path = path;

				printf("\033[30;1H");
				cout << endl << "break3 " << endl;
				printf("\033[1;1H");
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