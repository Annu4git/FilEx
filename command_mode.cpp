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

void select_command(terminal &app, vector <string> token_stream) {
	cout << "In select_command " << endl;
	if(token_stream.size() > 0) {
		for(auto i : token_stream) {
			cout << i << endl;
		}
		cout << "If passed " << endl;
		if(token_stream[0] == "copy") {
			cout << "Calling copy impl " << endl;
			copy_impl(app, token_stream[1], token_stream[2]);
		}
	}
}

void enter_in_command_mode(terminal &app) {

	app.set_cursor_position(34, 1, true);
	app.print_text("#######################################################################");
	app.new_line(); 
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
				token = token + c;
				token_stream.push_back(token);
				select_command(app, token_stream);
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