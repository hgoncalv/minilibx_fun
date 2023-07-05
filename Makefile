CC = gcc
CFLAGS = #-Wall -Wextra -Werror
LDFLAGS = -L./minilibx-linux -lmlx_Linux -lX11 -lXext -lm

SRC_DIR = src
SRCS := $(wildcard $(SRC_DIR)/*.c)
TARGETS := $(patsubst $(SRC_DIR)/%.c,%,$(SRCS))

all: $(TARGETS)

%: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean

