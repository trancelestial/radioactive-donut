src = $(wildcard *.c)
obj = $(src:.c=.o)

CC = gcc
LDFLAGS = -Wall -g -lm -lGL -lGLU -lglut 

radioactive_donut: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm radioactive_donut
	rm *.o
	rm *~
