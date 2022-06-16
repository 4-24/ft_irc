NAME = ircserv

CPP = c++ #-g

CPPFLAGS = -std=c++98 -Wall -Wextra -Werror

SRC_NAME =	main.cpp \
			Server.cpp \
			User.cpp \
			Socket.cpp \
			Message.cpp \
			Command.cpp \
			Room.cpp

OBJ = $(SRC_NAME:.cpp=.o)

RM = rm -f

%.o : %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(NAME) : $(OBJ)
	$(CPP) -o $(NAME) $(OBJ) $(CPPFLAGS)

all : $(NAME)

clean	:
	rm -rf $(OBJ)

fclean	: clean
	rm -rf $(NAME)

re		: fclean all

.PHONY: all clean fclean re
