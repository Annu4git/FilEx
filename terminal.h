#include <iostream>
#include <string>

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

	int cursor_position_y;

	terminal();

	void reset_cursor_position();

	void set_cursor_position();

	void move_cursor_up();

	void move_cursor_down();

	void move_cursor_right();

	void move_cursor_left();
};

#endif