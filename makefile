CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic

OBJ_DIR = obj
BIN_DIR = bin

CLIENT_TARGET = client
SERVER_TARGET = server

.PHONY: all debug clean
.SECONDARY:

all: $(BIN_DIR)/$(CLIENT_TARGET) $(BIN_DIR)/$(SERVER_TARGET)

debug: CFLAGS += -ggdb
debug: all

$(BIN_DIR)/%: $(OBJ_DIR)/%.o | $(BIN_DIR)
	$(CC) $< -o $@

$(OBJ_DIR)/%.o: %.c msg.h config.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)