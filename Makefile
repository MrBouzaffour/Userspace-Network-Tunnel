# Author: Ahmed Bouzaffour 2025-12-08

CC = gcc
CFLAGS = -wall -Wextra -Iinclude
LDFLAGS = -lsodium

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

all: $(BIN_DIR)/vpn

#Linking
$(BIN_DIR)/vpn: $(OBJ_DIR)/main.o $(OBJ_DIR)/tun.o $(OBJ_DIR)/net.o $(OBJ_DIR)/crypto.o
	$(CC) $(OBJ_DIR)/main.o $(OBJ_DIR)/tun.o $(OBJ_DIR)/net.o $(OBJ_DIR)/crypto.o -o $(BIN_DIR)/vpn $(LDFLAGS)

#Compiling
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/tun.o: $(SRC_DIR)/tun.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/tun.c -o $(OBJ_DIR)/tun.o

$(OBJ_DIR)/net.o: $(SRC_DIR)/net.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/net.c -o $(OBJ_DIR)/net.o

$(OBJ_DIR)/crypto.o: $(SRC_DIR)/crypto.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/crypto.c -o $(OBJ_DIR)/crypto.o

clean:
	rm -f $(OBJ_DIR)/main.o
	rm -f $(OBJ_DIR)/tun.o
	rm -f $(OBJ_DIR)/net.o
	rm -f $(OBJ_DIR)/crypto.o
	rm -f $(BIN_DIR)/vpn

.PHONY: all clean
