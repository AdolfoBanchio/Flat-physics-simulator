# simple makefile for the project
# based on the following compiler lines

# Variables
CC = g++
CFLAGS = -c
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

# Targets
all: sfml-app

sfml-app: $(OBJS)
	$(CC) $(OBJS) -o sfml-app $(LFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o sfml-app

run: all
	./sfml-app
# EOF