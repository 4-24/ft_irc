SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

NAME = ft_irc

FLAGS = -std=c++98 -Wall -Wextra -Werror
CLANG = clang++ $(FLAGS)

RM = rm -f

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CLANG) $(OBJS) -o $(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
