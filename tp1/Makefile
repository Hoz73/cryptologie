#TP1 Crypto
#Hamze AL RASHEED
#Rémi BOUVIER

CC = g++
CFLAGS = -Wall -pedantic -Werror -O3
EFLAGS = -lcrypto -lssl

main: main.o classes.o
	$(CC) $(CFLAGS) -o main main.o classes.o $(EFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

classes.o: classes.cpp
	$(CC) $(CFLAGS) -c classes.cpp