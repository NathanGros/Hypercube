# Usage:
# make # compile program
# make clean  # remove binary

.PHONY = all clean

CC = gcc
CFLAGS = -Wall
LDFLAGS = -lraylib -lm

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
TARGETNAME = main

all: $(TARGETNAME)

$(TARGETNAME): $(OBJS)
	@echo "Compiling $(TARGETNAME)..."
	@$(CC) $(OBJS) -o $(TARGETNAME) $(LDFLAGS)
	@rm $(OBJS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	@rm -rvf $(OBJS) ${TARGETNAME}
