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

string error_msg = "\033[1;31mError: \033[0m";
string too_few_args = "Too few arguments found! Try something like : ";

vector <string> break_string_into_tokens(terminal &app, string command_text);

void print_message(terminal &app, string message_to_user) {
	app.set_cursor_position(app.message_row_no, 1, true);
	string print_space = "";

	for(int i = 0; i < app.terminal_col; i++) {
		print_space = print_space + " ";
	}
	app.print_text(print_space);
	
	app.set_cursor_position(app.message_row_no, 1, true);
	app.print_text(message_to_user);
}


void reset_command_space(terminal &app) {
	app.set_cursor_position(app.fence_row_no, 1, true);

	string print_fence = "";

	for(int i = 0; i < app.terminal_col; i++) {
		print_fence = print_fence + "-";
	}

	string print_space = "";

	for(int i = 0; i < app.terminal_col; i++) {
		print_space = print_space + " ";
	}

	app.print_text(print_fence);
	app.new_line();
	app.print_text(print_space);
	app.set_cursor_position(app.command_row_no, 1, true);
	app.print_text(":");
}

void clear_command_space(terminal &app) {
	
	app.set_cursor_position(app.fence_row_no, 1, true);
	
	string print_space = "";
	for(int i = 0; i < app.terminal_col; i++) {
		print_space = print_space + " ";
	}
	app.print_text(print_space);
	
	app.new_line(); 
	
	print_space = "";
	for(int i = 0; i < app.terminal_col; i++) {
		print_space = print_space + " ";
	}
	app.print_text(print_space);

	app.new_line();
	
	print_space = "";
	for(int i = 0; i < app.terminal_col; i++) {
		print_space = print_space + " ";
	}
	app.print_text(print_space);
}

int select_command(terminal &app, vector <string> token_stream) {
	if(token_stream.size() > 0) {
		
		string command = token_stream[0];
		if(command[command.size() - 1] == '\n') {
				command.resize(command.size() - 1);

		}

		string message_to_user;

		if(command == "copy") {

			if(token_stream.size() < 3) {
				message_to_user = error_msg + too_few_args + "copy foo.txt bar.txt baz.mp4 ~/foobar";
			} else {
				string destination_path = token_stream[token_stream.size() - 1];
				destination_path = get_absolute_path_for_file(app, destination_path);
				copy_impl(app, token_stream, destination_path);
				message_to_user = "Copied successfully.";

			}

		} else if(command == "move") {

			if(token_stream.size() < 3) {
				message_to_user = error_msg + too_few_args + "move foo.txt bar.txt baz.mp4 ~/foobar";
			} else {
				string destination_path = token_stream[token_stream.size() - 1];
				destination_path = get_absolute_path_for_file(app, destination_path);
				move_impl(app, token_stream, destination_path);
				message_to_user = "Moved successfully.";

			}

		} else if  (command == "rename") {

			if(token_stream.size() < 3) {
				message_to_user = error_msg + too_few_args + "rename foo.txt bar.txt";
			} else {

				string old_name = get_absolute_path_for_file(app, token_stream[1]);
				string new_name = get_absolute_path_for_file(app, token_stream[2]);

				rename_impl(app, old_name, new_name);

				message_to_user = "Renamed successfully.";
			}

		} else if (command == "create_file") {

			if(token_stream.size() < 3) {
				message_to_user = error_msg + too_few_args + "create_file foo.txt ~/foobar";
			} else {

				string file_to_be_created = token_stream[1];
				string directory_path = token_stream[2];

				create_file_impl(app, file_to_be_created, directory_path);
				message_to_user = "File created successfully.";
			}

		} else if (command == "create_dir") {

			if(token_stream.size() < 3) {
				message_to_user = error_msg + too_few_args + "create_dir folder_name ~/foobar";
			} else {

				string directory_to_be_created = token_stream[1];
				string directory_path = token_stream[2];
				create_dir_impl(app, directory_to_be_created, directory_path);
				message_to_user = "Directory created successfully.";
			}

		} else if (command == "delete_file") {

			if(token_stream.size() < 2) {
				message_to_user = error_msg + too_few_args + "delete_file <file_path>";
			} else {

				string file_to_be_deleted = token_stream[1];
				file_to_be_deleted = get_absolute_path_for_file(app, file_to_be_deleted);
				delete_file_impl(app, file_to_be_deleted);
				message_to_user = "File deleted successfully.";
			}

		} else if (command == "delete_dir") {

			if(token_stream.size() < 2) {
				message_to_user = error_msg + too_few_args + "delete_dir <directory_path>";
			} else {

				string directory_to_be_deleted = token_stream[1];
				directory_to_be_deleted = get_absolute_path_for_file(app, directory_to_be_deleted);
				delete_directory_impl(app, directory_to_be_deleted);
				message_to_user = "Directory deleted successfully.";
			}

		} else if (command == "goto") {

			if(token_stream.size() < 2) {
				message_to_user = error_msg + too_few_args + "goto <directory_path>";
			} else {

				string directory_path = token_stream[1];

				if(directory_path[0] == '~') {
					directory_path = app.root_path + "/" + directory_path ;
				} else if(directory_path[0] == '/') {

				} else {
					directory_path = app.root_path + "/" + directory_path ;
				}
				
				directory_path = get_absolute_path(app, directory_path);

				enter_into_directory(app, directory_path, "command-goto", "");

				message_to_user = "Opening directory";

				print_message(app, message_to_user);

				return 0;
			}

		} else if(command == "search") {

			app.search_results = 0;

			if(token_stream.size() < 2) {
				message_to_user = error_msg + too_few_args + "search <filename>";
			} else {

				vector < tuple < string, string, char > > search_result;
				string search_query = token_stream[1];
				vector < tuple < string, string, char > > searched_list = 
				enter_into_directory(app, app.current_path, "command-search", search_query);

				if(searched_list.size() == 0) {
					message_to_user = "No results found!";

					print_message(app, message_to_user);
					return -1;
				} else {
					message_to_user = "Match(es) found!";

					print_message(app, message_to_user);
					return 0;
				}
				
			}

			
			
		} else if(command == "snapshot") {

			if(token_stream.size() < 3) {
				message_to_user = error_msg + too_few_args + "snapshot <folder> <dumpfile>";
			} else {

				snapshot_impl(app, token_stream[1], token_stream[2]);

				message_to_user = "Snapshoting done.";
			}

		} else {

			string temp = ":";
			string to_print = command + temp;
			temp = " command not found";
			to_print = to_print + temp;
			
			message_to_user = to_print;

		}

		print_message(app, message_to_user);

	}

	return 0;
	
}

string enter_in_command_mode(terminal &app) {

	string command_text = "";
	vector <string> token_stream;

	bool start_new_command = true;

	while(1) {

		if(start_new_command == true) {
			reset_command_space(app);
		}	

		char c = getchar();

		int k = 0;
		if(c == 27) {	/* This is for escape to return */
			
			clear_command_space(app);
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
				fflush(stdout);
				app.set_cursor_position(true);
			}

		} else {
			(app.cursor_position_y)++;
			cout << c;
			fflush(stdout);

			if(c == '\n') {
				
				token_stream = break_string_into_tokens(app, command_text);
				
				if(token_stream.size() > 0) {
					string command = token_stream[0];
					
					int status = select_command(app, token_stream);
					
					if(command == "search") {
						if(status == -1) {

						} else {
							return "command-return-from-search";
						}

					} else if(command == "goto") {
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