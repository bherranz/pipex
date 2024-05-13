NAME = pipex
BONUS_NAME = .bonus
PROGRAM = pipexx

SRCS = main.c get_path.c

BONUS = bonus/main.c bonus/get_path.c

OBJS = ${SRCS:.c=.o}
BONUS_OBJS = ${BONUS:.c=.o}

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror
REMOVE = rm -f

all: ${NAME}

${NAME}: ${OBJS} $(LIBFT)
		${CC} ${CFLAGS} ${OBJS} -L$(LIBFT_DIR) -lft -o ${NAME}

bonus: ${BONUS_NAME}

${BONUS_NAME}: ${BONUS_OBJS} $(LIBFT)
		${CC} ${CFLAGS} ${BONUS_OBJS} -L$(LIBFT_DIR) -lft -o ${PROGRAM}

$(LIBFT):
		make -C $(LIBFT_DIR) all

clean:
		${REMOVE} ${OBJS} ${BONUS_OBJS}
		@make -sC $(LIBFT_DIR) clean

fclean: 
		${REMOVE} ${OBJS} ${BONUS_OBJS}
		${REMOVE} ${NAME} ${PROGRAM}
		@make -sC $(LIBFT_DIR) fclean

re: fclean all

clear:
	@clear

run: clear re
	./$(NAME) "8 6 5 7 3 2 1 4 9"

.PHONY:	all clean fclean re clear run bonus