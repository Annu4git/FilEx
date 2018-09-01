#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#include <unistd.h> /* to use chown : change owner */

#include "linux_cmd.h"
#include "terminal.h"
#include "file_utils.h"

using namespace std;

void delete_file_impl(string file_name) {
	remove(file_name.c_str());
}

void delete_directory_impl(string directory_name) {

}

void rename_impl(string old_file, string new_file) {
	
	if(new_file[new_file.size() - 1] == '\n') {
			new_file.resize(new_file.size() - 1);
	}
	
	rename(old_file.c_str(), new_file.c_str());
}

void move_impl(terminal &app, vector <string> token_stream) {
	string seperator = "/";

	string destination_file_location = token_stream[token_stream.size() - 1];

	for(int i = 1; i < token_stream.size() - 1; i++) {

		string source_file_path = token_stream[i];

		string destination_file_path = destination_file_location;

		destination_file_path = destination_file_path + seperator;
		destination_file_path = destination_file_path + source_file_path;

		rename_impl(source_file_path, destination_file_path);
	}

}

void copy_impl(terminal &app, vector <string> token_stream) {

	string seperator = "/";

	string destination_file_location = token_stream[token_stream.size() - 1];
	
	for(int i = 1; i < token_stream.size() - 1; i++) {

		string source_file_path = token_stream[i];

		tuple < string, string, char, string, unsigned int, unsigned int > file = get_file_by_name_from_current_directory(app, source_file_path);

		string permission = get<3>(file);

		string destination_file_path = destination_file_location;

		destination_file_path = destination_file_path + seperator;
		destination_file_path = destination_file_path + source_file_path;

		int source_file_handle, destination_file_handle;

		source_file_handle = open(source_file_path.c_str(), O_RDONLY);

		//destFD = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);


		destination_file_handle = open(destination_file_path.c_str(), O_WRONLY | O_CREAT);
		//destination_file_handle = open(destination_file_path.c_str(), O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		//destination_file_handle = open(destination_file_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		
		char *container[2048];

		int result;

		while( (result = read(source_file_handle, container, 2048) ) > 0) {
			write(destination_file_handle, container, result);
		}

		close(destination_file_handle);
		close(source_file_handle);

		unsigned int user = 0, group = 0, other = 0;

		if(permission[0] == 'r') {
			user += 4;
		}
		if(permission[1] == 'w') {
			user += 2;
		}
		if(permission[2] == 'x') {
			user += 1;
		}
		if(permission[3] == 'r') {
			group += 4;
		}
		if(permission[4] == 'w') {
			group += 2;
		}
		if(permission[5] == 'x') {
			group += 1;
		}
		if(permission[6] == 'r') {
			other += 4;
		}
		if(permission[7] == 'w') {
			other += 2;
		}
		if(permission[8] == 'x') {
			other += 1;
		}

		unsigned int all = 100*user + 10*group + other;
		string prefix = "0000";
		prefix = prefix + to_string(all);
		
		unsigned int permission_number = std::stoul (prefix,nullptr,0);

		unsigned int own = get<4>(file);

		unsigned int grp = get<5>(file);
		
		chmod(destination_file_path.c_str(), permission_number);

		if (chown(destination_file_path.c_str(), own, grp) == -1) {
	      	//cout << "User doesn't have permission to change ownership";
	  	}
	}

	
}

vector < tuple < string, string, char > > ls_impl(bool first_time, terminal &app) {

	string current_directory_path = get_current_directory_path();
	app.root_path = current_directory_path; /* only sets first time*/
	char current_working_directory[current_directory_path.length()+1]; 
    strcpy(current_working_directory, current_directory_path.c_str()); 
	vector < tuple < string, string, char > > file_list = show_and_get_file_list(current_working_directory, app);
	
	return file_list;
}

vector < tuple < string, string, char > > ls_impl(string current_directory_path, terminal &app) {

	//char current_working_directory[current_directory_path.length()+1]; 
    //strcpy(current_working_directory, current_directory_path.c_str()); 
	vector < tuple < string, string, char  > > file_list = show_and_get_file_list(current_directory_path, app);

	//fflush(stdin);

	return file_list;
}

