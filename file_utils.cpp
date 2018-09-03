/*

Name  	: Anurag Chaturvedi
Roll No : 2018201024

*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string>
#include <iostream>
#include <pwd.h>
#include <grp.h>
#include <bits/stdc++.h>
#include <cstring>
#include <tuple>

#include "file_utils.h"
#include "terminal.h"
#include "directory_utils.h"
#include "terminal_utils.h"

using namespace std;

void print_size(size_t file_size, terminal &app) {

	int multiple = 0;

	long unsigned int N = 1024;

	while(file_size > N) {
		multiple++;
		file_size /= N;
	}

	string unit;

	if(multiple == 0) {
		unit = "Bytes";
	} else if(multiple == 1) {
		unit = "KB   ";
	} else if(multiple == 2) {
		unit = "MB   ";
	} else if(multiple == 3) {
		unit = "GB   ";
	}

	string total = to_string(file_size);

	if(total.size() < 4) {
		cout << file_size;
		int remaining = 4 - total.size();
		while(remaining--) {
			cout << " ";
		}
		cout << " " << unit << " ";
	} else {
		cout << file_size << " " << unit << " ";
	}

	fflush(stdout);
}

/* most important function to display list */
vector < tuple < string, string, char > > show_and_get_file_list(string current_directory_path, 
	terminal &app) {

	clear_terminal();

	app.set_cursor_position(1, 1);

	string file_title = "File / Directory";
	string size_title = "Size";
	string owner_title = "Owner";
	string group_title = "Group";
	string permissions_title = "Permissions";
	string last_modified_title = "Last Modified";

	cout << file_title << "              " << size_title << "       "
	<< owner_title << "  " << group_title << "     "
	<< permissions_title << "  " << last_modified_title ;

	app.reset_cursor_position();

	DIR *current_directory;
	
	struct dirent **files;
	struct stat current_stat;
	struct stat *current_time;
	struct passwd *pw;
	struct group  *gr;

	/* to store file in a tuple list */
	vector < tuple < string, string, char > > file_list;

	int number_of_records = scandir(current_directory_path.c_str(), &files, NULL, alphasort);

	app.total_records_in_current_directory = number_of_records;

	int i = app.index_of_first_record_to_be_displayed;
	int total_records_to_show = app.index_of_first_record_to_be_displayed + app.total_records_to_be_displayed;

	if(i < number_of_records && i > -1) {

		for(; i < total_records_to_show && i < number_of_records; i++) {

			string file_name = files[i]->d_name;

			string qualified_file_name = current_directory_path;
			string seprator = "/";

			qualified_file_name = qualified_file_name + seprator;
			qualified_file_name = qualified_file_name + file_name;

			stat(file_name.c_str(), &current_stat);

			if(file_name.size() > 30) {
				cout << file_name.substr(0, 25) << "...  ";
			} else if(file_name.size() < 30) {
				int remaining = 30 - file_name.size();
				cout << file_name;
				while(remaining--) {
					cout << " ";
				}
			}

			print_size(current_stat.st_size, app);

			pw = getpwuid(current_stat.st_uid);
			gr = getgrgid(current_stat.st_gid);

			cout << pw->pw_name << " " << gr->gr_name << " ";

			char is_directory = (files[i]->d_type == DT_DIR) ? 'd' : '-';

			char is_read_permission_for_user = (current_stat.st_mode & S_IRUSR) ? 'r' : '-';
			char is_write_permission_for_user = (current_stat.st_mode & S_IWUSR) ? 'w' : '-';
			char is_execute_permission_for_user = (current_stat.st_mode & S_IXUSR) ? 'x' : '-';

			char is_read_permission_for_group = (current_stat.st_mode & S_IRGRP) ? 'r' : '-';
			char is_write_permission_for_group = (current_stat.st_mode & S_IWGRP) ? 'w' : '-';
			char is_execute_permission_for_group = (current_stat.st_mode & S_IXGRP) ? 'x' : '-';

			char is_read_permission_for_others = (current_stat.st_mode & S_IROTH) ? 'r' : '-';
			char is_write_permission_for_others = (current_stat.st_mode & S_IWOTH) ? 'w' : '-';
			char is_execute_permission_for_others = (current_stat.st_mode & S_IXOTH) ? 'x' : '-';
			
			cout << is_directory << " ";

			string permissions = "";
			permissions = permissions + is_read_permission_for_user;
			permissions = permissions + is_write_permission_for_user;
			permissions = permissions + is_execute_permission_for_user;

			permissions = permissions + is_read_permission_for_group;
			permissions = permissions + is_write_permission_for_group;
			permissions = permissions + is_execute_permission_for_group;

			permissions = permissions + is_read_permission_for_others;
			permissions = permissions + is_write_permission_for_others;
			permissions = permissions + is_execute_permission_for_others;

			file_list.push_back(make_tuple(file_name, qualified_file_name, is_directory));

			cout << "  " << permissions << "  ";

			cout << " ";

			char date[10];
			strftime(date, 20, "%d-%m-%y", localtime(&(current_stat.st_mtime)));

			 cout << date;

			cout << endl;

			fflush(stdout);
		}
	}

	app.reset_cursor_position();

	fflush(stdout);

	return file_list;
}

/* not using now */
tuple < string, string, char, string, unsigned int, unsigned int > 
get_file_by_name_from_current_directory(terminal &app, string given_file_name) {

	string current_directory_path = get_current_directory_path();
	
	char current_working_directory[current_directory_path.length()+1]; 
    strcpy(current_working_directory, current_directory_path.c_str());

	DIR *current_directory;
	
	struct dirent **files;
	struct stat current_stat;
	struct passwd *pw;
	struct group  *gr;

	/* filename, full path, file_type, file_permissions, user, group */
	tuple < string, string, char, string, unsigned int, unsigned int > file;

	int number_of_records = scandir(current_directory_path.c_str(), &files, NULL, alphasort);

	for(int i = 0; i < number_of_records; i++) {
		string file_name = files[i]->d_name;
		if (file_name == given_file_name) {


			string qualified_file_name = current_directory_path;
			string seprator = "/";

			qualified_file_name = qualified_file_name + seprator;
			qualified_file_name = qualified_file_name + file_name;

			stat(file_name.c_str(), &current_stat);

			pw = getpwuid(current_stat.st_uid);
			gr = getgrgid(current_stat.st_gid);

			char is_directory = (files[i]->d_type == DT_DIR) ? 'd' : '-';
			
			char is_read_permission_for_user = (current_stat.st_mode & S_IRUSR) ? 'r' : '-';
			char is_write_permission_for_user = (current_stat.st_mode & S_IWUSR) ? 'w' : '-';
			char is_execute_permission_for_user = (current_stat.st_mode & S_IXUSR) ? 'x' : '-';

			char is_read_permission_for_group = (current_stat.st_mode & S_IRGRP) ? 'r' : '-';
			char is_write_permission_for_group = (current_stat.st_mode & S_IWGRP) ? 'w' : '-';
			char is_execute_permission_for_group = (current_stat.st_mode & S_IXGRP) ? 'x' : '-';

			char is_read_permission_for_others = (current_stat.st_mode & S_IROTH) ? 'r' : '-';
			char is_write_permission_for_others = (current_stat.st_mode & S_IWOTH) ? 'w' : '-';
			char is_execute_permission_for_others = (current_stat.st_mode & S_IXOTH) ? 'x' : '-';

			string permissions = "";
			permissions = permissions + is_read_permission_for_user;
			permissions = permissions + is_write_permission_for_user;
			permissions = permissions + is_execute_permission_for_user;

			permissions = permissions + is_read_permission_for_group;
			permissions = permissions + is_write_permission_for_group;
			permissions = permissions + is_execute_permission_for_group;

			permissions = permissions + is_read_permission_for_others;
			permissions = permissions + is_write_permission_for_others;
			permissions = permissions + is_execute_permission_for_others;

			file = make_tuple(file_name, qualified_file_name, is_directory, 
				permissions, current_stat.st_uid, current_stat.st_gid);

			char date[10];
			strftime(date, 20, "%d-%m-%y", localtime(&(current_stat.st_ctime)));

			return file;
		}
	}
}

/* for copy command : copy recursively */
tuple < string, string, char, string, unsigned int, unsigned int > 
	get_file_by_name_from_given_directory(terminal &app, 
	string current_directory_path, string given_file_name) {

	char current_working_directory[current_directory_path.length()+1]; 
    strcpy(current_working_directory, current_directory_path.c_str());

	DIR *current_directory;
	
	struct dirent **files;
	struct stat current_stat;
	struct passwd *pw;
	struct group  *gr;

	/* filename, full path, file_type, file_permissions, user, group */
	tuple < string, string, char, string, unsigned int, unsigned int > file;

	int number_of_records = scandir(current_directory_path.c_str(), &files, NULL, alphasort);

	for(int i = 0; i < number_of_records; i++) {
		string file_name = files[i]->d_name;
		if (file_name == given_file_name) {


			string qualified_file_name = current_directory_path;
			string seprator = "/";

			qualified_file_name = qualified_file_name + seprator;
			qualified_file_name = qualified_file_name + file_name;

			stat(file_name.c_str(), &current_stat);

			pw = getpwuid(current_stat.st_uid);
			gr = getgrgid(current_stat.st_gid);

			char is_directory = (files[i]->d_type == DT_DIR) ? 'd' : '-';
			
			char is_read_permission_for_user = (current_stat.st_mode & S_IRUSR) ? 'r' : '-';
			char is_write_permission_for_user = (current_stat.st_mode & S_IWUSR) ? 'w' : '-';
			char is_execute_permission_for_user = (current_stat.st_mode & S_IXUSR) ? 'x' : '-';

			char is_read_permission_for_group = (current_stat.st_mode & S_IRGRP) ? 'r' : '-';
			char is_write_permission_for_group = (current_stat.st_mode & S_IWGRP) ? 'w' : '-';
			char is_execute_permission_for_group = (current_stat.st_mode & S_IXGRP) ? 'x' : '-';

			char is_read_permission_for_others = (current_stat.st_mode & S_IROTH) ? 'r' : '-';
			char is_write_permission_for_others = (current_stat.st_mode & S_IWOTH) ? 'w' : '-';
			char is_execute_permission_for_others = (current_stat.st_mode & S_IXOTH) ? 'x' : '-';

			string permissions = "";
			permissions = permissions + is_read_permission_for_user;
			permissions = permissions + is_write_permission_for_user;
			permissions = permissions + is_execute_permission_for_user;

			permissions = permissions + is_read_permission_for_group;
			permissions = permissions + is_write_permission_for_group;
			permissions = permissions + is_execute_permission_for_group;

			permissions = permissions + is_read_permission_for_others;
			permissions = permissions + is_write_permission_for_others;
			permissions = permissions + is_execute_permission_for_others;

			file = make_tuple(file_name, qualified_file_name, is_directory, 
				permissions, current_stat.st_uid, current_stat.st_gid);

			return file;
		}
	}
}

/* returns file list and not show them */
vector < tuple < string, string, char, string, unsigned int, unsigned int > > 
get_file_list(string current_directory_path, terminal &app) {

	DIR *current_directory;
	
	struct dirent **files;
	struct stat current_stat;
	struct stat *current_time;
	struct passwd *pw;
	struct group  *gr;

	/* to store file in a tuple list */
	vector < tuple < string, string, char, string, unsigned int, unsigned int > > file_list;

	int number_of_records = scandir((current_directory_path).c_str(), &files, NULL, alphasort);

	for(int i = 0; i < number_of_records; i++) {

		string file_name = files[i]->d_name;

		string qualified_file_name = current_directory_path;
		string seprator = "/";

		qualified_file_name = qualified_file_name + seprator;
		qualified_file_name = qualified_file_name + file_name;

		stat(file_name.c_str(), &current_stat);

		char is_directory = (files[i]->d_type == DT_DIR) ? 'd' : '-';

		char is_read_permission_for_user = (current_stat.st_mode & S_IRUSR) ? 'r' : '-';
		char is_write_permission_for_user = (current_stat.st_mode & S_IWUSR) ? 'w' : '-';
		char is_execute_permission_for_user = (current_stat.st_mode & S_IXUSR) ? 'x' : '-';

		char is_read_permission_for_group = (current_stat.st_mode & S_IRGRP) ? 'r' : '-';
		char is_write_permission_for_group = (current_stat.st_mode & S_IWGRP) ? 'w' : '-';
		char is_execute_permission_for_group = (current_stat.st_mode & S_IXGRP) ? 'x' : '-';

		char is_read_permission_for_others = (current_stat.st_mode & S_IROTH) ? 'r' : '-';
		char is_write_permission_for_others = (current_stat.st_mode & S_IWOTH) ? 'w' : '-';
		char is_execute_permission_for_others = (current_stat.st_mode & S_IXOTH) ? 'x' : '-';

		string permissions = "";
		permissions = permissions + is_read_permission_for_user;
		permissions = permissions + is_write_permission_for_user;
		permissions = permissions + is_execute_permission_for_user;

		permissions = permissions + is_read_permission_for_group;
		permissions = permissions + is_write_permission_for_group;
		permissions = permissions + is_execute_permission_for_group;

		permissions = permissions + is_read_permission_for_others;
		permissions = permissions + is_write_permission_for_others;
		permissions = permissions + is_execute_permission_for_others;
		
		file_list.push_back(make_tuple(file_name, qualified_file_name, is_directory, 
			permissions, current_stat.st_uid, current_stat.st_gid));

	}

	return file_list;
}


/* for search command only*/
/* displays file search result */
vector < tuple < string, string, char > > show_and_get_file_list_with_search(string current_directory_path, 
	terminal &app, string search_query) {

	clear_terminal();

	app.set_cursor_position(1, 1);

	string file_title = "File / Directory";

	cout << file_title ;

	fflush(stdout);

	app.reset_cursor_position();

	DIR *current_directory;
	
	struct dirent **files;
	struct stat current_stat;
	struct stat *current_time;

	/* to store file in a tuple list */
	vector < tuple < string, string, char > > file_list;

	int number_of_records = scandir(current_directory_path.c_str(), &files, NULL, alphasort);

	for(int i = 0; i < number_of_records; i++) {

		string file_name = files[i]->d_name;
	
		if (file_name == search_query) {

		app.search_results = app.search_results + 1;
		
		string qualified_file_name = current_directory_path;
		string seprator = "/";

		qualified_file_name = qualified_file_name + seprator;
		qualified_file_name = qualified_file_name + file_name;

		stat(file_name.c_str(), &current_stat);

		if(file_name.size() > 30) {
			cout << file_name.substr(0, 25) << "...  ";
		} else if(file_name.size() < 30) {
			int remaining = 30 - file_name.size();
			cout << file_name;
			while(remaining--) {
				cout << " ";
			}
		}

		char is_directory = (files[i]->d_type == DT_DIR) ? 'd' : '-';

		file_list.push_back(make_tuple(file_name, qualified_file_name, is_directory));

		cout << endl;

		fflush(stdout);

		}
	
	}

	fflush(stdout);

	app.reset_cursor_position();

	return file_list;
}

