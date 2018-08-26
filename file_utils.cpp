#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string>
#include<iostream>
#include"file_utils.h"

using namespace std;

void showFiles(char *current_directory_path) {
	DIR *current_directory;
	
	struct dirent *current_file;
	struct stat current_stat;

	current_directory = opendir(current_directory_path);

	while((current_file = readdir(current_directory)) != NULL) {
		stat(current_file->d_name, &current_stat);
		cout << current_stat.st_size << " " ;
		cout << current_file->d_name << endl;
	}

	closedir(current_directory);
}
