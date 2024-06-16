CC = gcc
CFLAGS  = -g -Wall
default: program5

program5: program5.o
	$(CC) $(CFLAGS) -o program5 program5.o -lm

program5.o: program5.c
	$(CC) $(CFLAGS) -c program5.c

clean:
	$(RM) program5 *.o *~ output.txt
