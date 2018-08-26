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

#include "file_utils.h"

using namespace std;

void print_size(size_t file_size) {

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
		unit = "KB";
	} else if(multiple == 2) {
		unit = "MB";
	} else if(multiple == 3) {
		unit = "GB";
	}

	cout << file_size << " " << unit << " ";
}

vector<string> show_and_get_file_list(char *current_directory_path) {
	DIR *current_directory;
	
	struct dirent *current_file;
	struct stat current_stat;
	struct passwd *pw;
	struct group  *gr;

	/* to store file in a vector list */
	vector<string> file_list;

	current_directory = opendir(current_directory_path);

	while((current_file = readdir(current_directory)) != NULL) {
		char* file_name = current_file->d_name;

		string qualified_file_name(current_directory_path);
		string temp_file_name(file_name);
		string seprator = "/";

		qualified_file_name = qualified_file_name + seprator;
		qualified_file_name = qualified_file_name + temp_file_name;
		
		file_list.push_back(qualified_file_name);

		stat(file_name, &current_stat);

		cout << file_name << " ";

		print_size(current_stat.st_size);

		pw = getpwuid(current_stat.st_uid);
		gr = getgrgid(current_stat.st_gid);

		cout << pw->pw_name << " " << gr->gr_name << " ";

		

		char is_directory = (S_ISDIR(current_stat.st_mode)) ? 'd' : '-';

		char is_read_permission_for_user = (current_stat.st_mode & S_IRUSR) ? 'r' : '-';
		char is_write_permission_for_user = (current_stat.st_mode & S_IWUSR) ? 'w' : '-';
		char is_execute_permission_for_user = (current_stat.st_mode & S_IXUSR) ? 'x' : '-';

		char is_read_permission_for_group = (current_stat.st_mode & S_IRGRP) ? 'r' : '-';
		char is_write_permission_for_group = (current_stat.st_mode & S_IWGRP) ? 'w' : '-';
		char is_execute_permission_for_group = (current_stat.st_mode & S_IXGRP) ? 'x' : '-';

		char is_read_permission_for_others = (current_stat.st_mode & S_IROTH) ? 'r' : '-';
		char is_write_permission_for_others = (current_stat.st_mode & S_IWOTH) ? 'w' : '-';
		char is_execute_permission_for_others = (current_stat.st_mode & S_IXOTH) ? 'x' : '-';
		
		cout << is_read_permission_for_user << is_write_permission_for_user << is_execute_permission_for_user;
		cout << is_read_permission_for_group << is_write_permission_for_group << is_execute_permission_for_group;
		cout << is_read_permission_for_others << is_write_permission_for_others << is_execute_permission_for_others;

		//cout << " " << localtime(&(current_stat.st_ctime));
		cout << " ";

		char date[10];
		strftime(date, 20, "%d-%m-%y", localtime(&(current_stat.st_ctime)));
		cout << date;

		cout << endl;

	}

	closedir(current_directory);

	return file_list;
}
