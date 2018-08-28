CC = g++

CFLAGS = -Wall -std=c++1z

DEPS = terminal.h terminal_utils.h directory_utils.h file_utils.h linux_cmd.h

OBJ = terminal.o terminal_utils.o directory_utils.o file_utils.o linux_cmd.o filExApp.o

%.o: %.cpp ($DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

filEx : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
