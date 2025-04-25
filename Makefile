NAME = pipex
BONUS_NAME = .bonus
PROGRAM = pipex

SRCS = src/main.c src/get_path.c

BONUS = bonus/main_bonus.c bonus/get_path_bonus.c bonus/here_doc_bonus.c \
		bonus/gnl/get_next_line.c bonus/gnl/get_next_line_utils.c \
		bonus/processes_bonus.c

OBJS = ${SRCS:.c=.o}
BONUS_OBJS = ${BONUS:.c=.o}

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror
REMOVE = rm -f

all: ${NAME}

${NAME}: ${OBJS} $(LIBFT)
		@${CC} ${CFLAGS} ${OBJS} -L$(LIBFT_DIR) -lft -o ${NAME}

bonus: ${BONUS_NAME}

${BONUS_NAME}: ${BONUS_OBJS} $(LIBFT)
		@touch $(BONUS_NAME)
		@${CC} ${CFLAGS} ${BONUS_OBJS} -L$(LIBFT_DIR) -lft -o ${PROGRAM}

$(LIBFT):
		@make -C $(LIBFT_DIR) all

clean:
		${REMOVE} ${OBJS} ${BONUS_OBJS}
		@make -sC $(LIBFT_DIR) clean
		@rm -rf $(BONUS_NAME)

fclean: 
		@${REMOVE} ${OBJS} ${BONUS_OBJS}
		@${REMOVE} ${NAME} ${PROGRAM}
		@make -sC $(LIBFT_DIR) fclean
		@rm -rf $(BONUS_NAME)

re: fclean all

clear:
	@clear

.PHONY:	all clean fclean re clear bonus
