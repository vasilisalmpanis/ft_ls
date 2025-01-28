NAME	= ft_ls
SRC	= ./src/main.c
OBJ	= $(SRC:.c=.o)
RM	= rm
CC	= clang
CFLAGS	= -Wall -Wextra -Werror
INC	= ./include

all: $(NAME)

$(NAME) : libft $(OBJ) 
	@if [ -n "$(OBJ)" ]; then \
		$(CC) $(CFLAGS) -I $(INC) $(OBJ) ./libft/libft.a -o $(NAME); \
	fi

%.o: %.c
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

clean:
	make clean -C ./libft/
	$(RM) $(OBJ)

fclean: clean
	make fclean -C ./libft/
	$(RM) $(NAME)

libft:
	make -C ./libft

re: fclean all

clang-format:
	clang-format -i $(SRC) $(INC)/ft_ls.h

.PHONY: clean fclean re libft
