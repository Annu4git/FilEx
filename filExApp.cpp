#include <iostream>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>

#include "linux_cmd.h"
#include "terminal_utils.h"
#include "terminal.h"

using namespace std;

terminal app;

int main() {

	keyboard_settings_off();

	clear_terminal();

	app.reset_cursor_position();

	vector < tuple < string, string, char > >  file_list = ls_impl(true, app);	// defined in linux_cmd
	
	app.current_path = app.root_path;

	app.set_cursor_position();

	app.increment_trace_pointer();
	
	app.trace[app.trace_pointer] = app.root_path;
	
	hold_terminal(file_list, app);

	fflush(stdin);

	keyboard_settings_on();

	return 0;	
}

