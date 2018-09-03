/*

Name  	: Anurag Chaturvedi
Roll No : 2018201024

*/

#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string>
#include <iostream>
#include <string>

#include "directory_utils.h"

std::string get_current_directory_path() {

	char cwd[PATH_MAX];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       
       return std::string(cwd);
   } else {
       
		return std::string("error");
   }
}

