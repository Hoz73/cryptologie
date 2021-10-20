CC = g++
CFLAGS = -Wall -pedantic -Werror -O3

main: main.o classes.o
	$(CC) $(CFLAGS) -o main main.o classes.o -lcrypto -lssl

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

classes.o: classes.cpp
	$(CC) $(CFLAGS) -c classes.cpp