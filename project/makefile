CC		= gcc

CFLAGS		= -g

LDFLAGS		= -g

OUT		= test

OBJS		= test.o listmalloc.o

main: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUT)
	./test

clean:
	rm -f *.o sh core out
