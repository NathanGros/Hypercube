# Usage:
# make # compile program
# make clean  # remove binary

.PHONY = all clean

CC = gcc
CFLAGS = -Wall
LDFLAGS = -lraylib -lm

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
TARGETNAME = hypergraphs

all: $(TARGETNAME)

$(TARGETNAME): $(OBJS)
	@echo "Compiling $(TARGETNAME)..."
	@$(CC) $(OBJS) -o $(TARGETNAME) $(LDFLAGS)
	@rm $(OBJS)
	@echo "Done."

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

windows:
	mkdir -pv hypergraphs_windows
	@echo "Compiling $(TARGETNAME).exe for Windows..."
	x86_64-w64-mingw32-gcc $(CFLAGS) -o hypergraphs_windows/$(TARGETNAME).exe $(SRCS) -lraylib -lopengl32 -lgdi32 -lwinmm
	@echo "Copying DLLs..."
	@cp -v /usr/x86_64-w64-mingw32/bin/libraylib.dll hypergraphs_windows/
	@cp -v /usr/x86_64-w64-mingw32/bin/libgcc_s_seh-1.dll hypergraphs_windows/
	@cp -v /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll hypergraphs_windows/
	@cp -v /usr/x86_64-w64-mingw32/bin/libssp-0.dll hypergraphs_windows/
	@echo "Making zip folder..."
	zip -j hypergraphs_windows.zip hypergraphs_windows/*
	@echo "Done."


clean:
	@echo "Cleaning up..."
	@rm -rvf $(OBJS) ${TARGETNAME}
	@echo "Cleaning up windows build..."
	@rm -rvf hypergraphs_windows/*
	@rmdir -v hypergraphs_windows
	@rm -rvf hypergraphs_windows.zip
	@echo "Done."
