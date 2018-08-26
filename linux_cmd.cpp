#include "linux_cmd.h"

void ls_impl() {

	string str = get_current_directory_path();
	char cwd[str.length()+1]; 
    strcpy(cwd, str.c_str()); 
	showFiles(cwd);

	fflush(stdin);
}

