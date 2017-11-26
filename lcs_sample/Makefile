CC := clang

CFLAGS := -std=gnu99 -O3

ifeq ($(DEBUG),1)
        CFLAGS += -O0
else
        CFLAGS += -O3 -DNDEBUG
endif

all: lcs_run

lcs_run: lcs_run.c
	$(CC) $(CFLAGS) lcs_run.c lcs_implementation.c lcs_cache_oblivious.c -o lcs

clean:
	$(RM) lcs_run *.o .buildmode
