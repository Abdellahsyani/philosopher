NAME = philo

SRC = main.c handle_philo.c utils_philo.c parser.c helper_philo.c

CC = cc
CFLAG = -Wall -Wextra -Werror #-fsanitize=thread

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAG) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
