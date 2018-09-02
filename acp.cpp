#include<iostream>

using namespace std;

int main() {

	string search_query = "Dir";
	string file_name = "Dir";

	int flag = 1;

	for(int i = 0; i < search_query.size(); i++) {
		if(search_query[i] != file_name[i]) {
			flag = 0;
			break;
		}
	}

	if(flag == 1) {
		cout << "Match";
	} else {
		cout << "Not match";
	}
}