NAME	= ft_ls
SRC	= ./src/main.c	\
	  ./src/utils.c \
	  ./src/sort.c

OBJ	= $(SRC:.c=.o)
RM	= rm
CC	= clang
CFLAGS	= -Wall -Wextra -Werror
INC	= ./include

all: $(NAME)

$(NAME) : libftprintf $(OBJ) 
	@if [ -n "$(OBJ)" ]; then \
		$(CC) $(CFLAGS) -I$(INC) -I./ft_printf $(OBJ) ./ft_printf/libftprintf.a -o $(NAME); \
	fi

%.o: %.c
	$(CC) $(CFLAGS) -I$(INC) -I./ft_printf -c $< -o $@

clean:
	make clean -C ./ft_printf
	$(RM) $(OBJ)

fclean: clean
	make fclean -C ./ft_printf
	$(RM) $(NAME)

libftprintf:
	make -C ./ft_printf

re: fclean all

.PHONY: clean fclean re libft libftprintf
