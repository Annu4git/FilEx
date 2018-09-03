/*

Name  	: Anurag Chaturvedi
Roll No : 2018201024

*/

1. Normal Mode :

1) Showing file size in Bytes, KB, MB and GB (assuming no file be greater than 1023 GB)
Handled vertical overflow with scrolling, if window size would be small (vertically) then also it will adjust.
2) But atleast 8 rows (vertically) required to show the content.
3) Horizontally atleast 70 columns are required to run the application correctly (approximatel half the screen).
4) Clicking on . (current directory), will refresh the file listing.
5) Clicking on .. will take the control to parent directory (not beyond root directory of application).
6) Left Key : To go back
   Right Key : To go forward
   Backspace : To go one level up (parent directory)
   H : To go to home directory of application

2. Command Mode :
	
1) If less arguments will be given (less than minimum required for the particular command),
an error message (in red color) will say that "too few arguments found".
It will also give suggestion, how to provide arguments.
2) Copying and moving files and directory ensures that file permissions and the ownership will be intact.
3) In copy, move, rename, create_file, create_dir, delete_file, delete_dir : relative and absolute path will work.
4) Goto command will require absolute path.
5) Search command will search in current and following child directories.
6) Snapshot command : Creates dump file in root directory of application.
7) Every command will give a message to user, below the command.
8) On pressing escape : control will go to Normal mode and clear the command space.
