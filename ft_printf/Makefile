NAME	=	libftprintf.a
SRC	=	ft_printf.c \
		ft_utilities.c \
		ft_auxiliary.c

OBJ	=	${SRC:.c=.o}
LIBC	=	ar -rc
RM	=	rm -rf
INCS	=	"ft_printf.h"
CC	=	clang
CFLAGS	=	-Wall -Wextra -Werror 

all : libft ${NAME}

${NAME} : ${OBJ}
		cp libft/libft.a .
		mv libft.a ${NAME}
		${LIBC} ${NAME} ${OBJ}

${OBJ}	:
		${CC} ${CFLAGS} -c ${SRC}

libft:
		make -C libft
clean :
		${RM} ${OBJ}
		${MAKE} clean -C libft

fclean : clean
		${RM} ${NAME}
		${MAKE} fclean -C libft
		
re :	fclean all

.PHONY: all clean fclean re libft
