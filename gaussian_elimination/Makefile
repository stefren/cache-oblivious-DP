CC := clang

CFLAGS := -std=gnu99 -O3

ifeq ($(DEBUG),1)
        CFLAGS += -O0
else
        CFLAGS += -O3 -DNDEBUG
endif

all: gaussian_elimination

gaussian_elimination: gaussian_elimination_with_gep.c
	$(CC) $(CFLAGS) ../gep.c gaussian_elimination_with_gep.c -o gaussian_elimination 

clean:
	$(RM) gaussian_elimination *.o .buildmode