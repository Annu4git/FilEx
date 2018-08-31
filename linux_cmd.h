#include <string>
#include <bits/stdc++.h>

#include "file_utils.h"
#include "directory_utils.h"
#include "terminal.h"

#ifndef INC_LINUX_CMD_H
#define INC_LINUX_CMD_H

using namespace std;

void copy_impl(terminal &app, string source_file_path, string destination_file_path);

vector < tuple < string, string, char > > ls_impl(bool first_time, terminal &app);

vector < tuple < string, string, char > > ls_impl(string current_directory_path, terminal &app);

#endif
