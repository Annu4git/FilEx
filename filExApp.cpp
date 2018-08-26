#include <iostream>
#include <string>
#include <bits/stdc++.h>

#define clear_terminal() printf("\033[H\033[J")

#include "linux_cmd.h"

using namespace std;

int main() {

	clear_terminal();

	ls_impl();

	while(1);
	fflush(stdin);
	return 0;	
}
