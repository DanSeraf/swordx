CC = gcc
SRC_DIR = src
CFLAGS = -Wall -g -I. -Iinclude                                             

SRC = src/{args.c,bintree.c,linkedstack.c,log.c,swordx.c,trie.c}
OUT = swordx


default:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)


.PHONY: all clean

clean:
	rm -f $(OUT)
