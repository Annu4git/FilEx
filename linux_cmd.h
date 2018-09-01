#include <string>
#include <bits/stdc++.h>

#include "file_utils.h"
#include "directory_utils.h"
#include "terminal.h"

#ifndef INC_LINUX_CMD_H
#define INC_LINUX_CMD_H

using namespace std;

void copy_impl(terminal &app, vector <string> token_stream);

void move_impl(terminal &app, vector <string> token_stream);

void rename_impl(string old_file, string new_file);

void delete_file_impl(string file_name);

void delete_directory_impl(string directory_name);

vector < tuple < string, string, char > > ls_impl(bool first_time, terminal &app);

vector < tuple < string, string, char > > ls_impl(string current_directory_path, terminal &app);

#endif
