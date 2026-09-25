NAME    := codexion
CC		:= cc -g
CFLAGS  := -Wall -Wextra -Werror -pthread
RM      := rm -rf
INC		:= -I include

# ThreadSanitizer flags for leak check
TSAN_FLAGS	:=	-fsanitize=thread

# directories
BUILD_DIR 	:=	build
LOG_DIR		:= 	logs
SRC_DIR		:= 	src
TEST_DIR	:=	tests

# log path
LOG_PATH	?= $(LOG_DIR)/Codexion-$(shell date +%s).log

# executable arguments
ARGS	?= 5 2000 200 200 200 5 0 FIFO


SRCS	:= $(SRC_DIR)/main.c \
			$(SRC_DIR)/coder.c \
			$(SRC_DIR)/coder_action.c \
			$(SRC_DIR)/coderedf.c \
			$(SRC_DIR)/errors.c \
			$(SRC_DIR)/monitor.c \
			$(SRC_DIR)/queue.c \
			$(SRC_DIR)/queue_utils.c \
			$(SRC_DIR)/setup.c \
			$(SRC_DIR)/thread_utils.c \
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

run:
	./$(NAME) $(ARGS)

log: $(LOG_DIR)
	$(MAKE) run ARGS="$(ARGS)" > $(LOG_PATH)

$(LOG_DIR):
		mkdir -p $(LOG_DIR)

logclean:
		$(RM) $(LOG_DIR)

clean:
		$(RM) $(BUILD_DIR)

fclean: clean logclean
		$(RM) $(NAME)

re: fclean all

.PHONY: clean logclean fclean re
