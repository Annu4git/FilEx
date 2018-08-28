#include <string>
#include <bits/stdc++.h>

#include "file_utils.h"
#include "directory_utils.h"

#ifndef INC_LINUX_CMD_H
#define INC_LINUX_CMD_H

using namespace std;

vector < tuple < string, string, char > > ls_impl(bool first_time, string &root_path);

vector < tuple < string, string, char > > ls_impl(string current_directory_path);

#endif