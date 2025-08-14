CC = gcc
CFLAGS = -Wall -Wextra  -O0 -g

BUILD_DIR = build
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC))

TARGET = slang

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $^

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
