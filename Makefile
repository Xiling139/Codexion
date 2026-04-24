NAME    := codexion
CC		:= cc
CFLAGS  := -Wall -Wextra -Werror -pthread -I include
RM      := rm -f

# Source paths
SRC_DIR	:= src

SRCS	:= $(SRC_DIR)/main.c \
			$(SRC_DIR)/errors.c \
			$(SRC_DIR)/utils.c

OBJS    := $(SRCS:.c=.o)

# -----------------------------------------------------------------------------

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
