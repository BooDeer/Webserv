CC 		= c++
NAME 	= webserv
FLAGS 	= -std=c++98 -g\
		# -Wall -Wextra -Werror
DFLAGS 	= -std=c++98 -Wall -Wextra -Werror -g \
			-fsanitize=address
HEADER	= parsing/parsing.hpp parsing/configFile/Config.hpp
FILE	= webserv.cpp parsing/parsing.cpp parsing/configFile/Config.cpp

UNAME_S	= $(shell uname -s)

all: $(NAME)
	@echo "Creating the program..."


$(NAME): $(FILE) $(HEADER)
ifeq ($(UNAME_S),Linux)
	g++ $(FLAGS) $(FILE) -o $(NAME)
else ifeq ($(UNAME_S), Darwin)
	c++ $(FLAGS) $(FILE) -o $(NAME)
endif


t:
	@c++ test.cpp -o test
	@./test
.PHONY: clean re fclean all

re: clean all
	@echo "Recreating the program..."

clean:
	@echo "Deleting the program..."
	@rm -f $(NAME)
fclean: clean