CC = g++
CFLAGS = -Wall -pedantic -Werror -O3

main: main.o
	$(CC) $(CFLAGS) -o main main.o -lcrypto -lssl

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp