OLEVEL = -O2
CFLAGS = -Wall -Wextra -Wpedantic -std=c99 $(OLEVEL)

ifeq ($(TARGET), windows)
    CC = x86_64-w64-mingw32-gcc
    LIBS = -lws2_32
    EXT = .exe
else
    CC = gcc
    LIBS = 
    EXT = 
endif

OBJ_DIR = obj
BIN_DIR = bin

CLIENT_TARGET = client$(EXT)
SERVER_TARGET = server$(EXT)

.PHONY: all debug clean
.SECONDARY:

all: $(BIN_DIR)/$(CLIENT_TARGET) $(BIN_DIR)/$(SERVER_TARGET)

debug: CFLAGS := $(filter-out $(OLEVEL), $(CFLAGS)) -ggdb
debug: all

$(BIN_DIR)/%$(EXT): $(OBJ_DIR)/%.o | $(BIN_DIR)
	$(CC) $< -o $@ $(LIBS)

$(OBJ_DIR)/%.o: %.c msg.h compat.h config.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)