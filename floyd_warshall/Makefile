CC := clang

CFLAGS := -std=gnu99 -O3

ifeq ($(DEBUG),1)
        CFLAGS += -O0
else
        CFLAGS += -O3 -DNDEBUG
endif

all: floyd_warshall 

floyd_warshall: floyd_warshall_with_gep.c
	$(CC) $(CFLAGS) ../gep.c floyd_warshall_with_gep.c -o floyd_warshall 

clean:
	$(RM) floyd_warshall *.o .buildmode