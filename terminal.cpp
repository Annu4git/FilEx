/*

Name  	: Anurag Chaturvedi
Roll No : 2018201024

*/

#include <iostream>
#include <string>
#include <sys/ioctl.h>


#include "terminal.h"

using namespace std;

terminal::terminal() {

	root_path = "";

	current_path = "";

	trace_pointer = -1;

	for(int i = 0; i < 10000; i++) {
		trace[i] = "0";
	}

	cursor_position_x = 3;

	cursor_position_y = 1;

	index_of_first_record_to_be_displayed = 0;

	total_records_in_current_directory = 0;

	ioctl(0,TIOCGWINSZ, & terminal_size);

	set_window_parameters();

}

void terminal::set_window_parameters() {
	terminal_row = terminal_size.ws_row;
	terminal_col = terminal_size.ws_col;

	fence_row_no = terminal_row-2;
	command_row_no = terminal_row -1;
	message_row_no = terminal_row;

	total_records_to_be_displayed = terminal_row - 6;
}

void terminal::reset_index_of_first_record_to_be_displayed() {
	index_of_first_record_to_be_displayed = 0;
}

void terminal::reset_cursor_position() {
	cursor_position_x = 3;
	cursor_position_y = 1;
	printf("\033[%d;%df", cursor_position_x, cursor_position_y);
}

void terminal::set_cursor_position() {
	if(cursor_position_x > 2 && cursor_position_x < (total_records_in_current_directory + 3) 
		&& cursor_position_x < (total_records_to_be_displayed + 3) && cursor_position_y > 0) {
		printf("\033[%d;%dH", cursor_position_x, cursor_position_y);
	}
}

void terminal::set_cursor_position(int x, int y) {
	cursor_position_x = x;
	cursor_position_y = y;
	if(cursor_position_x > 2 && cursor_position_x < (total_records_in_current_directory + 3) 
		&& cursor_position_x < (total_records_to_be_displayed + 3) && cursor_position_y > 0) {
		printf("\033[%d;%dH", cursor_position_x, cursor_position_y);
	}
}



int terminal::move_cursor_up() {
	if(cursor_position_x > 3) {
		cursor_position_x -- ;
		set_cursor_position();
		return 0;
	}
	return 1;
}

int terminal::move_cursor_down() {
	if(cursor_position_x < (total_records_to_be_displayed + 2) 
		&& cursor_position_x < (total_records_in_current_directory + 2)) {
		cursor_position_x ++ ;
		set_cursor_position();
		return 0;
	}
	return 1;
}

void terminal::move_cursor_right() {
	cursor_position_y ++ ;
	set_cursor_position();
}

void terminal::move_cursor_left() {
	cursor_position_y -- ;
	set_cursor_position();
}

void terminal::increment_trace_pointer() {
	trace_pointer++;
}

void terminal::decrement_trace_pointer() {
	trace_pointer--;
}

void terminal::printTrace() {
	for(int i=0; i < 10; i++) {
		cout << trace[i] << "-->";
	}
}


/****************** specially for command mode **********************/

void terminal::new_line() {
	cursor_position_x ++ ;
	cursor_position_y = 1 ;
	cout << endl;
}

void terminal::print_text(string text) {
	cursor_position_y += text.size() ;
	cout << text;
}

void terminal::set_cursor_position(bool command_mode) {
	printf("\033[%d;%dH", cursor_position_x, cursor_position_y);
}

void terminal::set_cursor_position(int x, int y, bool command_mode) {
	cursor_position_x = x;
	cursor_position_y = y;
	printf("\033[%d;%dH", cursor_position_x, cursor_position_y);
}
/****************** specially for command mode **********************/