CC = gcc
HEADER		=	./include/
CFLAGS = -Wall -Wextra -Werror -g -I $(HEADER)  -fsanitize=address
LDFLAGS = -L./minilibx-linux -lmlx_Linux -lX11 -lXext -lm   -lmlx -L/usr/include/../lib -lXext -lX11 -lm 

LIBFT		=	lib/libft.a

SRC_DIR = src
SRCS := $(wildcard $(SRC_DIR)/*.c)
TARGETS := $(patsubst $(SRC_DIR)/%.c,%,$(SRCS))

all: $(TARGETS)

%: $(LIBFT) $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBFT) $(LDFLAGS)

$(LIBFT)	:
				make -C ./lib

clean:
	rm -rf $(TARGETS)

fclean		:	clean
				rm -rf $(TARGETS)
				make fclean -C ./lib

.PHONY: all clean

