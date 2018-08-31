#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#include "linux_cmd.h"
#include "terminal.h"

using namespace std;

void copy_impl(terminal &app, string source_file_path, string destination_file_path){
	cout << "Copy is working" << endl;

	int source_file_handle, destination_file_handle;

	source_file_handle = open(source_file_path.c_str(), O_RDONLY);
	destination_file_handle = open(destination_file_path.c_str(), O_WRONLY | O_CREAT);

	char *container[2048];

	int result;

	while( (result = read(source_file_handle, container, 2048) ) > 0) {
		cout << "Writing is working" << endl;
		write(destination_file_handle, container, result);
	}

	cout << "Writing is done" << endl;

	close(destination_file_handle);
	close(source_file_handle);
}

vector < tuple < string, string, char > > ls_impl(bool first_time, terminal &app) {

	string current_directory_path = get_current_directory_path();
	app.root_path = current_directory_path; /* only sets first time*/
	char current_working_directory[current_directory_path.length()+1]; 
    strcpy(current_working_directory, current_directory_path.c_str()); 
	vector < tuple < string, string, char > > file_list = show_and_get_file_list(current_working_directory, app);

	
	//fflush(stdin);

	
	return file_list;
}

vector < tuple < string, string, char > > ls_impl(string current_directory_path, terminal &app) {

	//char current_working_directory[current_directory_path.length()+1]; 
    //strcpy(current_working_directory, current_directory_path.c_str()); 
	vector < tuple < string, string, char > > file_list = show_and_get_file_list(current_directory_path, app);

	//fflush(stdin);

	return file_list;
}

