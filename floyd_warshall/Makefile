CC := clang

CFLAGS := -std=gnu99 -O3

ifeq ($(DEBUG),1)
        CFLAGS += -O0
else
        CFLAGS += -O3 -DNDEBUG
endif

all: test 
 
test: floyd_warshall_test.c
	$(CC) $(CFLAGS) ../gep.c floyd_warshall_with_gep.c floyd_warshall_naive.c floyd_warshall_test.c -o floyd_warshall_test 

clean:
	$(RM) floyd_warshall_test *.o .buildmode