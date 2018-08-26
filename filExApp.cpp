#include<iostream>
#include<string>
#include <bits/stdc++.h>

#define clear_terminal() printf("\033[H\033[J")

#include"file_utils.h"
#include "directory_utils.h"

using namespace std;

int main() {

	clear_terminal();

	string str = get_current_directory_path();
	char cwd[str.length()+1]; 
    strcpy(cwd, str.c_str()); 
	showFiles(cwd);

	fflush(stdin);

	while(1);
	fflush(stdin);
	return 0;	
}
