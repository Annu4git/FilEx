#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string>
#include <iostream>
#include <string>

#include "directory_utils.h"

std::string get_current_directory_path() {

//std :: cout << std::endl << "Entering directory path function" << std::endl << std::endl;

	char cwd[PATH_MAX];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       //std :: cout << std::endl << "Exiting directory path function" << std::endl << std::endl;
       return std::string(cwd);
   } else {
       //std :: cout << std::endl << "Exiting directory path function with error" << std::endl;
		return std::string("error");
   }
}

