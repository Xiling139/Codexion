NAME    := codexion
CC		:= cc -g
CFLAGS  := -Wall -Wextra -Werror -pthread
RM      := rm -rf
INC		:= -I include

# directories
BUILD_DIR 	:=	build
SRC_DIR		:= 	src

SRCS	:= $(SRC_DIR)/main.c \
			$(SRC_DIR)/coder.c \
			$(SRC_DIR)/errors.c \
			$(SRC_DIR)/monitor.c \
			$(SRC_DIR)/setup.c \
			$(SRC_DIR)/utils.c \
			$(SRC_DIR)/utils2.c

OBJS    := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# -----------------------------------------------------------------------------

$(NAME): $(OBJS)
		$(CC) $(OBJS) -o $(NAME)

all: $(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
		$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(BUILD_DIR):
		mkdir -p $(BUILD_DIR)

clean:
		$(RM) $(BUILD_DIR)

fclean: clean
		$(RM) $(NAME)

re: fclean all
