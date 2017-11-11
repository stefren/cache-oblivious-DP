CC := clang

CFLAGS := -std=gnu99 -O3

all: lcs_run

lcs_run: lcs_run.c
	$(CC) $(CFLAGS) lcs_run.c lcs_implementation.c -o lcs
