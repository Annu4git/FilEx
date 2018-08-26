#include <bits/stdc++.h>

#include "linux_cmd.h"

using namespace std;

vector<string> ls_impl() {

	string current_directory_path = get_current_directory_path();
	char current_working_directory[current_directory_path.length()+1]; 
    strcpy(current_working_directory, current_directory_path.c_str()); 
	vector<string> file_list = show_and_get_file_list(current_working_directory);

	fflush(stdin);

	return file_list;
}

vector<string> ls_impl(string current_directory_path) {

	char current_working_directory[current_directory_path.length()+1]; 
    strcpy(current_working_directory, current_directory_path.c_str()); 
	vector<string> file_list = show_and_get_file_list(current_working_directory);

	fflush(stdin);

	return file_list;
}

