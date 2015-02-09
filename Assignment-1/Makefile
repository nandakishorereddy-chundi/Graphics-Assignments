CC = g++
CFLAGS = -Wall
PROG = play

SRCS = main.cpp
LIBS = -lglut -lGL -lGLU --std=c++11

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
