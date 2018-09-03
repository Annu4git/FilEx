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

	app.set_window_parameters();

	vector < tuple < string, string, char > >  file_list = ls_impl(true, app);	// defined in linux_cmd
	
	app.current_path = app.root_path;

	app.current_file_list = file_list;

	app.set_cursor_position();

	app.increment_trace_pointer();

	app.trace[app.trace_pointer] = app.root_path;
	
	hold_terminal(app);

	fflush(stdout);

	keyboard_settings_on();

	return 0;	
}

