#include <iostream>
#include <string>

#include "terminal.h"

using namespace std;

terminal::terminal() {

	root_path = "";

	current_path = "";

	trace_pointer = -1;

	cursor_position_x = 1;

	cursor_position_y = 1;

}

void terminal::reset_cursor_position() {
	cursor_position_x = 1;
	cursor_position_y = 1;
	printf("\033[%d;%dH", cursor_position_x, cursor_position_y);
}

void terminal::set_cursor_position() {
	if(cursor_position_x > 0 && cursor_position_y > 0) {
		printf("\033[%d;%dH", cursor_position_x, cursor_position_y);
	}
}

void terminal::move_cursor_up() {
	cursor_position_x -- ;
	if(cursor_position_x > 0 && cursor_position_y > 0) {
		printf("\033[%d;%dH", cursor_position_x, cursor_position_y);
	}
}

void terminal::move_cursor_down() {
	cursor_position_x ++ ;
	if(cursor_position_x > 0 && cursor_position_y > 0) {
		printf("\033[%d;%dH", cursor_position_x, cursor_position_y);
	}
}

void terminal::move_cursor_right() {
	cursor_position_y ++ ;
	if(cursor_position_x > 0 && cursor_position_y > 0) {
		printf("\033[%d;%dH", cursor_position_x, cursor_position_y);
	}
}

void terminal::move_cursor_left() {
	cursor_position_y -- ;
	if(cursor_position_x > 0 && cursor_position_y > 0) {
		printf("\033[%d;%dH", cursor_position_x, cursor_position_y);
	}
}