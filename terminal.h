#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <sys/ioctl.h>


using namespace std;

#ifndef INC_TERMINAL_H
#define INC_TERMINAL_H

class terminal {

public:

	string root_path;

	string current_path;

	string trace[10000];

	int trace_pointer;

	int cursor_position_x;

	int cursor_position_y;;

	int index_of_first_record_to_be_displayed;

	int total_records_to_be_displayed;

	int total_records_in_current_directory;

	struct winsize terminal_size;

	int terminal_row;

	int terminal_col;

	int fence_row_no;

	int command_row_no;

	int message_row_no;

	vector < tuple < string, string, char > > current_file_list;

	terminal();

	void set_window_parameters();

	void reset_index_of_first_record_to_be_displayed();

	void reset_cursor_position();

	void set_cursor_position();

	void set_cursor_position(bool command_mode);

	void set_cursor_position(int x, int y);

	void set_cursor_position(int x, int y, bool command_mode);

	int move_cursor_up();

	int move_cursor_down();

	void move_cursor_right();

	void move_cursor_left();

	void increment_trace_pointer();

	void decrement_trace_pointer();

	void printTrace();

	/*****************************/

	void new_line();

	void print_text(string text);
	
	/*****************************/
};

#endif