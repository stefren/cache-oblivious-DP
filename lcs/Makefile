CC := clang

CFLAGS := -std=gnu99 -O3

ifeq ($(DEBUG),1)
        CFLAGS += -O0
else
        CFLAGS += -O3 -DNDEBUG
endif

all: lcs_test

lcs_test: lcs_test.c
	$(CC) $(CFLAGS) ../gep.c lcs_naive.c lcs_cache_oblivious.c lcs_test.c -o lcs

clean:
	$(RM) lcs_run *.o .buildmode
