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

vector <string> break_string_into_tokens(terminal &app, string command_text);

void print_message(terminal &app, string message_to_user) {
	app.print_text("                                                                       ");
	app.set_cursor_position(36, 1, true);
	app.print_text(message_to_user);
}

void select_command(terminal &app, vector <string> token_stream) {
	if(token_stream.size() > 0) {
		
		string command = token_stream[0];
		if(command[command.size() - 1] == '\n') {
				command.resize(command.size() - 1);

		}

		string message_to_user;

		if(command == "search") {
			vector < tuple < string, string, char > > search_result;
			string search_query = token_stream[1];
			search_impl(app, app.current_path, search_query, search_result);

			if(search_result.size() > 0) {

				clear_terminal();

				app.set_cursor_position(1, 1);

				string file_title = "File / Directory";
				string size_title = "Size";
				string owner_title = "Owner";
				string group_title = "Group";
				string permissions_title = "Permissions";
				string last_modified_title = "Last Modified";

				cout << file_title << "              " << size_title << "       "
				<< owner_title << "  " << group_title << "     "
				<< permissions_title << "  " << last_modified_title ;

				app.reset_cursor_position();

				for (auto it : search_result) {
					cout << get<1>(it) << endl;
				}

				app.reset_cursor_position();

			}
		} else if(command == "copy") {

			copy_impl(app, token_stream);
			message_to_user = "Copied successfully.";

		} else if(command == "move") {

			move_impl(app, token_stream);
			message_to_user = "Moved successfully.";

		} else if  (command == "create_file") {

			create_file_impl(app, token_stream[1], token_stream[2]);
			message_to_user = "File created successfully.";

		} else if  (command == "create_dir") {

			create_dir_impl(app, token_stream[1], token_stream[2]);
			message_to_user = "Directory created successfully.";

		} else if  (command == "rename") {

			rename_impl(app, token_stream[1], token_stream[2]);
			message_to_user = "Renamed successfully.";

		} else if (command == "delete_file") {

			delete_file_impl(app, token_stream[1]);
			message_to_user = "Deleted successfully.";

		} else if (command == "delete_dir") {

			delete_directory_impl(app, token_stream[1]);
			message_to_user = "Deleted successfully.";

		} else if (command == "goto") {

			string directory_path = token_stream[1];

			if(directory_path[0] == '~') {
				directory_path = app.root_path + "/" + directory_path ;
			} else if(directory_path[0] == '/') {

			} else {
				directory_path = app.root_path + "/" + directory_path ;
			}
			
			directory_path = get_absolute_path(app, directory_path);

			enter_into_directory(app, directory_path, "command-goto");

			return;

		} else if(command == "snapshot") {

			snapshot_impl(app, token_stream[1], token_stream[2]);

			message_to_user = "Snapshoting done.";

		} else {

			string temp = ":";
			string to_print = command + temp;
			temp = " command not found";
			to_print = to_print + temp;
			
			message_to_user = to_print;

		}

		print_message(app, message_to_user);

	}

	return;
	
}

string enter_in_command_mode(terminal &app) {

	string command_text = "";
	vector <string> token_stream;

	bool start_new_command = true;

	while(1) {

		if(start_new_command == true) {
			app.set_cursor_position(34, 1, true);
			app.print_text("#######################################################################");
			app.new_line();
			app.print_text("                                                                       ");
			app.set_cursor_position(35, 1, true);
			app.print_text(":");
		}	

		char c = getchar();

		int k = 0;
		if(c == 27) {	/* This is for escape to return */
			
			app.set_cursor_position(34, 1, true);
			app.print_text("                                                                       ");
			app.new_line(); 
			app.print_text(" ");
			app.new_line();
			app.print_text("                                                                       ");
			command_text = "";
			start_new_command = true;
			return "command";
		} 

		if(c == 127) {	/* This is for backspace */
			if(app.cursor_position_y > 2) {
				command_text.resize(command_text.size() - 1);
				(app.cursor_position_y)--;
				app.set_cursor_position(true);
				cout << " ";
				app.set_cursor_position(true);
			}

		} else {
			(app.cursor_position_y)++;
			cout << c;

			if(c == '\n') {
				
				token_stream = break_string_into_tokens(app, command_text);
				
				if(token_stream.size() > 0) {
					string command = token_stream[0];
					
					select_command(app, token_stream);
					
					if(token_stream[0] == "goto") {
						start_new_command = 0;
						return "";
					}
				}

				command_text = "";
				start_new_command = true;
			}
			else {
				command_text = command_text + c;
				start_new_command = false;
			}

		}
	}
}

vector <string> break_string_into_tokens(terminal &app, string command_text) {
	
	string token = "", msg;
	vector <string> token_stream;

	for(int i = 0; i < command_text.size(); i ++) {
		
		if(command_text[i] == ' ') {
			token_stream.push_back(token);
			token = "";
		} else {
			token = token + command_text[i];
		}
	}

	token_stream.push_back(token);

	return token_stream;
}