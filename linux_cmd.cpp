#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <queue>

#include <unistd.h> /* to use chown : change owner */

#include "linux_cmd.h"
#include "terminal.h"
#include "file_utils.h"
#include "terminal_utils.h"

using namespace std;


void copy_file_to_directory(tuple < string, string, char, string, 
	unsigned int, unsigned int > file,
	 string destination_path, terminal &app) {

	string file_name = get<0>(file);
	string source_file_path = get<1>(file);
	char file_type = get<2>(file);
	string permission = get<3>(file);
	unsigned int own = get<4>(file);
	unsigned int grp = get<5>(file);

	//string msg = "In copy finally : source_file_path : ";
	//msg = msg + source_file_path + " and destination_file_path : ";
	//msg = msg + destination_path;
	string msg = "Finally" + source_file_path;
	debug(app, msg);

	string destination_file_path = destination_path + "/" + file_name;

	int source_file_handle, destination_file_handle;

	source_file_handle = open(source_file_path.c_str(), O_RDONLY);

	destination_file_handle = open(destination_file_path.c_str(), O_WRONLY | O_CREAT);
	
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
	
	chmod(destination_file_path.c_str(), permission_number);

	if (chown(destination_file_path.c_str(), own, grp) == -1) {
      	//cout << "User doesn't have permission to change ownership";
  	}
}



void copy_recursively(terminal &app, string source_path, 
	string source_file_name, string destination_path) {

	string msg = "In copy_recursively, current_directory_path : " + source_path
	+ " & source_file_name : " + source_file_name + "";
	debug(app, msg);

	tuple < string, string, char, string, unsigned int, unsigned int > source_file 
	= get_file_by_name_from_given_directory(app, source_path, source_file_name);

	char source_file_type = get<2>(source_file);

	//string new_current_file_or_directory_path = current_directory_path + "/" +file_name;

	//string new_destination_file_location = destination_file_location + file_name;

	if(source_file_name == "." || source_file_name == "..") {
		msg = "Ignore for : " + source_file_name;
		debug(app, msg);
		return;
	}
	if(source_file_type == 'd') {

		string source_directory_name = source_file_name;

		create_dir_impl(app, source_directory_name, destination_path);

		msg = "directory found and with name : " + source_file_name;
		debug(app, msg);
		msg = "directory created in : " + destination_path;
		debug(app, msg);


		/* since we found a directory so source path 
		and destination path will be updated
		current directory path + / + source directory name
	 	*/
		string new_destination_path = destination_path + "/" + source_directory_name;
		string new_source_path = source_path + "/" + source_directory_name;

		msg = "get files for : " + new_source_path;
		debug(app, msg);

		vector < tuple < string, string, char > > files_in_directory = 
		get_file_list(new_source_path);

		msg = "files : " + files_in_directory.size();
		debug(app, msg);

		for(auto it : files_in_directory) {

			string new_source_file_name = get<0>(it);

			msg = "in files loop, a file found with name : " + new_source_file_name;
			debug(app, msg);

			copy_recursively(app, new_source_path,
			new_source_file_name, new_destination_path);
		}


	} else {

		/* if not directory but a file is found we will call our copy function
			we have destination path and source name along with path
			that will work
			1. actual file to be copied, it will have every value needed
			2. destination path
			3. app
		*/
		copy_file_to_directory(source_file, destination_path, app);
	}
}


void copy_impl(terminal &app, vector <string> token_stream, 
	string destination_path) {

	string msg = "In copy impl";
	debug(app, msg);

	destination_path = app.root_path + "/" + destination_path;

	for(int i = 1; i < token_stream.size() - 1; i++) {
		copy_recursively(app, app.current_path, token_stream[i], destination_path);
	}
}

void move_impl(terminal &app, vector <string> token_stream) {
	string seperator = "/";

	string destination_file_location = token_stream[token_stream.size() - 1];

	for(int i = 1; i < token_stream.size() - 1; i++) {

		string source_file_path = token_stream[i];

		string destination_file_path = destination_file_location;

		destination_file_path = destination_file_path + seperator;
		destination_file_path = destination_file_path + source_file_path;

		debug(app, destination_file_path);
		rename_impl(app, source_file_path, destination_file_path);
	}

}

void rename_impl(terminal &app, string old_file, string new_file) {
	
	if(new_file[new_file.size() - 1] == '\n') {
			new_file.resize(new_file.size() - 1);
	}

	debug(app, old_file);
	debug(app, new_file);
	
	rename(old_file.c_str(), new_file.c_str());
}



void create_file_impl(terminal &app, string file_name, string directory_path) {
	
	if(directory_path[directory_path.size() - 1] == '\n') {
			directory_path.resize(directory_path.size() - 1);
	}

	directory_path = get_absolute_path(app, directory_path);

	string destination_file_path = directory_path;
	string seperator = "/";

	destination_file_path = destination_file_path + seperator;
	destination_file_path = destination_file_path + file_name;

	int destination_file_handle = open(destination_file_path.c_str(),O_WRONLY | O_CREAT | O_TRUNC, 
		S_IRUSR | S_IWUSR | S_IRGRP |S_IWGRP | S_IROTH | S_IWOTH);

}

void create_dir_impl(terminal &app, string directory_name, string directory_path) {
	
	if(directory_path[directory_path.size() - 1] == '\n') {
			directory_path.resize(directory_path.size() - 1);
	}

	directory_path = get_absolute_path(app, directory_path);

	string destination_directory_path = directory_path;
	string seperator = "/";

	destination_directory_path = destination_directory_path + seperator;
	destination_directory_path = destination_directory_path + directory_name;

	mkdir(destination_directory_path.c_str(), 0777);
	
}


void delete_file_impl(terminal &app, string file_name) {

	string msg = "Inside delete impl delete : " + file_name;
	debug(app, msg);

	int status = remove(file_name.c_str());

	msg = "status : " + status;
	debug(app, msg);
}

void delete_directory_impl(terminal &app, string directory_name) {

	vector < tuple < string, string, char > > file_list = get_file_list(directory_name);

	string msg = "Inside delete impl" + directory_name;
	debug(app, msg);

	queue < string > directory_queue;

	for(auto it : file_list) {

		string file_name = get<0>(it);
		string file_path = get<1>(it);
		char file_type = get<2>(it);

		if(file_type == 'd') {

			if(file_name != "." && file_name != "..") {
				directory_queue.push(file_path);

				msg = "Pushing directory_name : " + get<1>(it);
				debug(app, msg);
			}
		} else {
			delete_file_impl(app, file_path);
			//remove(file_name.c_str());

			msg = "removing file _name : " + file_name + " path : " + get<1>(it);
				debug(app, msg);
		}

		while(!directory_queue.empty()) {
			string directory_to_be_deleted = directory_queue.front();
			delete_directory_impl(app, directory_to_be_deleted);

			delete_file_impl(app, directory_to_be_deleted);

			//remove(directory_to_be_deleted.c_str());
			directory_queue.pop();

			msg = "removing directory _name : " + directory_to_be_deleted + " path : " + get<1>(it);
				debug(app, msg);
		}
	}

	delete_file_impl(app, directory_name);

	msg = "finally delete : " + directory_name;
				debug(app, msg);
}

void search_impl(terminal &app, string current_directory_path, string search_query, 
	vector < tuple < string, string, char > > &search_result) {

	vector < tuple < string, string, char > > file_list = get_file_list(current_directory_path);

	queue < string > directory_queue;

	for(auto it : file_list) {

		char file_type = get<2>(it);
		string file_name = get<0>(it);

		if(file_type == 'd') {

			if(file_name != "." && file_name != "..") {
				directory_queue.push(get<1>(it));
			}
		} else {

			size_t found = file_name.find(search_query);
  			if (found!=std::string::npos) {
				search_result.push_back(it);
  			}
			
		}

		while(!directory_queue.empty()) {
			search_impl(app, directory_queue.front(), search_query, search_result);
			directory_queue.pop();
		}
	}
}

void traverse_impl(terminal &app, string current_directory_path, 
	string dumpl_file_name, int count) {

	FILE *file = fopen(dumpl_file_name.c_str(), "a");

	if(count == 0) {
		create_file_impl(app, dumpl_file_name, app.root_path);
		count = 1;
	}

	vector < tuple < string, string, char > > file_list = get_file_list(current_directory_path);

	queue < string > directory_queue;

	fprintf(file, "%s:\n", current_directory_path.c_str());

	//fprintf(file, "%s:\t", current_directory_path);

	int line_counter = 1;

	for(auto it : file_list) {

		if(line_counter == 5) {
			fprintf(file, "%s\n");
			line_counter = 1;
		}

		char file_type = get<2>(it);

		if(file_type == 'd') {
			string file_name = get<0>(it);
			if(file_name != "." && file_name != ".." && file_name[0] != '.') {
				directory_queue.push(get<1>(it));
				fprintf(file, "%s\t", file_name.c_str());
				line_counter++;
			}
		} else {

			string file_name = get<0>(it);
			if(file_name[0] != '.') {
				fprintf(file, "%s\t", file_name.c_str());
				line_counter++;
			}	
		}
	}

	while(!directory_queue.empty()) {
			traverse_impl(app, directory_queue.front() , dumpl_file_name, count);
			directory_queue.pop();
		}

	fprintf(file, "\n\n");
	fclose(file);
}

void snapshot_impl(terminal &app, string directory_path_and_name, string dumpl_file_name) {
	
	dumpl_file_name = app.root_path + "/" + dumpl_file_name;
	traverse_impl(app, directory_path_and_name, dumpl_file_name, 0);

}


vector < tuple < string, string, char > > ls_impl(bool first_time, terminal &app) {

	string current_directory_path = get_current_directory_path();
	app.root_path = current_directory_path; /* only sets first time*/
	char current_working_directory[current_directory_path.length()+1]; 
    strcpy(current_working_directory, current_directory_path.c_str()); 
	vector < tuple < string, string, char > > file_list = 
	show_and_get_file_list(current_working_directory, app);
	
	return file_list;
}

vector < tuple < string, string, char > > ls_impl(string current_directory_path, terminal &app) {

	vector < tuple < string, string, char  > > file_list = 
	show_and_get_file_list(current_directory_path, app);

	return file_list;
}

vector < tuple < string, string, char > > ls_impl_with_search(string current_directory_path, 
	terminal &app, string search_query) {

	vector < tuple < string, string, char  > > file_list = 
	show_and_get_file_list_with_search(current_directory_path, app, search_query);

	return file_list;
}

string get_absolute_path(terminal &app, string path) {

	string absolute_path;

	if(path == "." || path == "./") {
		absolute_path = app.current_path;
				
	} else if(path == ".." || path == "../") {
		absolute_path = app.current_path;
		trim_path(absolute_path, app);
	} else if(path == "~" || path == "~/" || path == "/") {
		absolute_path = app.root_path;
	} else {
		absolute_path = path;
	}

	return absolute_path;
}