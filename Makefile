CFLAGS = -Wall -Wextra -Wpedantic -Iinclude
TARGET = build/pandora

CFILES = src/file.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): src/main.c | build
	gcc src/main.c $(CFILES) $(CFLAGS) -o $(TARGET)

clean:
	rm -rf build/*
