CC = g++

CFLAGS = -Wall -std=c++1z

DEPS = directory_utils.h file_utils.h linux_cmd.h

OBJ = directory_utils.o file_utils.o linux_cmd.o filExApp.o

%.o: %.cpp ($DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

filEx : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
