PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CC := gcc
CFLAGS := -Werror -Wall
LDFLAGS := -lrt -lpthread -g
BUILD_DIR := build

all: $(BUILD_DIR) player board

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

%: $(BUILD_DIR) %.c
	$(CC) $(CFLAGS) $*.c util.c -o $(BUILD_DIR)/$* $(LDFLAGS) -DSTART=start$*

run:
	./$(BUILD_DIR)/player

clean:
	rm -rf ASS4 $(BUILD_DIR)
