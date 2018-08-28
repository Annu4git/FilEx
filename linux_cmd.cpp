#include <bits/stdc++.h>

#include "linux_cmd.h"

using namespace std;

vector < tuple < string, string, char > > ls_impl(bool first_time, string &root_path) {

	string current_directory_path = get_current_directory_path();
	root_path = current_directory_path; /* only sets first time*/
	char current_working_directory[current_directory_path.length()+1]; 
    strcpy(current_working_directory, current_directory_path.c_str()); 
	vector < tuple < string, string, char > > file_list = show_and_get_file_list(current_working_directory);

	
	//fflush(stdin);

	
	return file_list;
}

vector < tuple < string, string, char > > ls_impl(string current_directory_path) {

	//char current_working_directory[current_directory_path.length()+1]; 
    //strcpy(current_working_directory, current_directory_path.c_str()); 
	vector < tuple < string, string, char > > file_list = show_and_get_file_list(current_directory_path);

	//fflush(stdin);

	return file_list;
}

