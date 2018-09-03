/*

Name  	: Anurag Chaturvedi
Roll No : 2018201024

*/

#include <string>
#include <bits/stdc++.h>

#include "file_utils.h"
#include "directory_utils.h"
#include "terminal.h"

#ifndef INC_LINUX_CMD_H
#define INC_LINUX_CMD_H

using namespace std;

void copy_impl(terminal &app, vector <string> token_stream, 
	string destination_file_location);

void move_impl(terminal &app, vector <string> token_stream, string destination_path);

void rename_impl(terminal &app, string old_file, string new_file);

void create_file_impl(terminal &app, string file_name, string directory_path);

void create_dir_impl(terminal &app, string directory_name, string directory_path);

void delete_file_impl(terminal &app, string file_name);

void delete_directory_impl(terminal &app, string directory_name);

void search_impl(terminal &app, string current_directory_path, string search_query, 
	vector < tuple < string, string, char, string, unsigned int, unsigned int > > &search_result);

void snapshot_impl(terminal &app, string directory_path_and_name, string dumpl_file_name);

vector < tuple < string, string, char > > ls_impl(bool first_time, terminal &app);

vector < tuple < string, string, char > > ls_impl(string current_directory_path, terminal &app);

vector < tuple < string, string, char > > ls_impl_with_search(string current_directory_path, 
	terminal &app, string search_query);

string get_absolute_path(terminal &app, string path);

string get_absolute_path_for_file(terminal &app, string file_name);

#endif
