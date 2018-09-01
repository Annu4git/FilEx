#include <bits/stdc++.h>

#ifndef INC_FILE_UTILS_H
#define INC_FILE_UTILS_H

#include "terminal.h"

using namespace std;

vector < tuple < string, string, char > > show_and_get_file_list(string s, terminal &app);

tuple < string, string, char, string, unsigned int, unsigned int > get_file_by_name_from_current_directory(terminal &app, string given_file_name);

#endif
