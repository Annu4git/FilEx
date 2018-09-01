#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string>
#include <bits/stdc++.h> 

#include "command_mode.h"
#include "terminal.h"
#include "terminal_utils.h"
#include "linux_cmd.h"

using namespace std;

void select_command(vector < tuple < string, string, char > > &file_list, 
	terminal &app, vector <string> token_stream) {
	if(token_stream.size() > 0) {
		
		string command = token_stream[0];
		if(command[command.size() - 1] == '\n') {
				command.resize(command.size() - 1);

		}

		if(command == "copy") {

			copy_impl(app, token_stream);
			app.print_text("                                                                       ");
			app.set_cursor_position(36, 1, true);
			app.print_text("Copied successfully.");

		} else if(command == "move") {

			move_impl(app, token_stream);
			app.print_text("                                                                       ");
			app.set_cursor_position(36, 1, true);
			app.print_text("Moved successfully.");

		} else if (command == "rename") {

			rename_impl(token_stream[1], token_stream[2]);
			app.print_text("                                                                       ");
			app.set_cursor_position(36, 1, true);
			app.print_text("Renamed successfully.");

		} else if (command == "delete_file") {

			delete_file_impl(token_stream[1]);
			app.print_text("                                                                       ");
			app.set_cursor_position(36, 1, true);
			app.print_text("Deleted successfully.");

		} else if (command == "delete_dir") {

			delete_directory_impl(token_stream[1]);

		} else if (command == "goto") {
			
			enter_into_directory(file_list, app, token_stream[1], "command");

			return;

		} else {

			string temp = ":";
			string to_print = command + temp;
			temp = " command not found";
			to_print = to_print + temp;
			app.print_text("                                                                       ");
			app.set_cursor_position(36, 1, true);
			app.print_text(to_print);

		}
	}

	enter_in_command_mode(file_list, app);
}

void enter_in_command_mode(vector < tuple < string, string, char > > file_list, terminal &app) {

	app.set_cursor_position(34, 1, true);
	app.print_text("#######################################################################");
	app.new_line();
	app.print_text("                                                                       ");
	app.set_cursor_position(35, 1, true);
	app.print_text(":");

	string token;
	vector <string> token_stream;

	while(1) {
		char c = getchar();

		int k = 0;
		if(c == 27) {
			
			app.set_cursor_position(34, 1, true);
			app.print_text("                                                                       ");
			app.new_line(); 
			app.print_text(" ");
			return;
		} 

		if(c == 127) {
			if(app.cursor_position_y > 2) {
				(app.cursor_position_y)--;
				app.set_cursor_position(true);
				cout << " ";
				app.set_cursor_position(true);
			}

		} else {
			(app.cursor_position_y)++;
			cout << c;

			if(c == '\n') {
				token_stream.push_back(token);
				select_command(file_list, app, token_stream);
				token = "";
			}
			else if(c != ' ') {
				token = token + c;
			} else {
				token_stream.push_back(token);
				token = "";
			}

		}

	}

	//app.reset_cursor_position();
}