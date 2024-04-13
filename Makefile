NAME = pipex
PROGRAM = pipex

SRCS = main.c

OBJS = ${SRCS:.c=.o}

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror
REMOVE = rm -f

all: ${NAME}

${NAME}: ${OBJS} $(LIBFT)
		${CC} ${CFLAGS} ${OBJS} -L$(LIBFT_DIR) -lft -o ${NAME}

$(LIBFT):
		make -C $(LIBFT_DIR) all

clean:
		${REMOVE} ${OBJS}
		@make -sC $(LIBFT_DIR) clean

fclean: 
		${REMOVE} ${OBJS}
		${REMOVE} ${NAME} ${PROGRAM}
		@make -sC $(LIBFT_DIR) fclean

re: fclean all

clear:
	@clear

.PHONY:	all clean fclean re clear run