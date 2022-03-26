CC 		= c++
NAME 	= webserv
FLAGS 	= -std=c++98 \
		# -Wall -Wextra -Werror
DFLAGS 	= -std=c++98 -Wall -Wextra -Werror -g \
			-fsanitize=address
HEADER	= 
FILE	= webserv.cpp

UNAME_S	= $(shell uname -s)

all: $(NAME)
	@echo "Creating the program..."
$(NAME): $(FILE) $(HEADER)
ifeq ($(UNAME_S),Linux)
	g++ $(FLAGS) $(FILE) -o $(NAME)
else ifeq ($(UNAME_S), Darwin) # must check the correct one for school.
	c++ $(FLAGS) $(FILE) -o $(NAME)
endif

.PHONY: clean re fclean all
re: clean all
clean:
	@echo "Deleting the program..."
	@rm -f $(NAME)
fclean: clean