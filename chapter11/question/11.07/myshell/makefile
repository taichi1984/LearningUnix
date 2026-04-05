CC = gcc
CFLAGS = -Wall -g
SRCS = smsh2.c loopflow.c parser.c process.c splitline.c stmt.c execute.c controlflow.c
OBJS = $(SRCS:.c=.o)

smsh2: $(OBJS)
	$(CC) $(OBJS) -o smsh2

%.o: %.c
	$(CC) $(CFLAGS) -c $<
