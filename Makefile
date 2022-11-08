# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eschirni <eschirni@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/27 14:45:54 by eschirni          #+#    #+#              #
#    Updated: 2022/11/07 16:38:23 by eschirni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# FLAGS
CFLAGS = -Wall -Wextra -Werror -std=c++98 -Wshadow -Wno-shadow

# COLORS
R = \033[31m
G = \033[32m
B = \033[34m
X = \033[0m

# EXECUTABLE
NAME = ./ircserv

# PATHS
SRC_PATH = ./
OBJ_PATH = ./obj/

# SOURCES
SRC = main.cpp

# OBJECTS
OBJ = $(patsubst $(SRC_PATH)%.c, $(OBJ_PATH)%.o, $(SRC))

# RULES
all: $(NAME)
	@echo "  $(R)iiii                                          "
	@echo " i::::i                                         "
	@echo "  iiii                                          \n"
	@printf "iiiiiii $(G)rrrrr   rrrrrrrrr        $(B)cccccccccccccccc\n"
	@echo "$(R)i:::::i $(G)r::::rrr:::::::::r     $(B)cc:::::::::::::::c"
	@echo " $(R)i::::i $(G)r:::::::::::::::::r   $(B)c:::::::::::::::::c"
	@echo " $(R)i::::i $(G)rr::::::rrrrr::::::r $(B)c:::::::cccccc:::::c"
	@echo " $(R)i::::i  $(G)r:::::r     r:::::r $(B)c::::::c     ccccccc"
	@echo " $(R)i::::i  $(G)r:::::r     rrrrrrr $(B)c:::::c             "
	@echo " $(R)i::::i  $(G)r:::::r             $(B)c:::::c             "
	@echo " $(R)i::::i  $(G)r:::::r             $(B)c::::::c     ccccccc"
	@echo "$(R)i::::::i $(G)r:::::r             $(B)c:::::::cccccc:::::c"
	@echo "$(R)i::::::i $(G)r:::::r              $(B)c:::::::::::::::::c"
	@echo "$(R)i::::::i $(G)r:::::r               $(B)cc:::::::::::::::c"
	@echo "$(R)iiiiiiii $(G)rrrrrrr                 $(B)cccccccccccccccc$(X)"

$(OBJ_PATH)%.o :$(SRC_PATH)%.c
	@echo $(Y)Compiling [$@]...$(X)
	@mkdir -p $(dir $@)
	@c++ $(CFLAGS) -c -o $@ $<
	@echo $(G)Finished [$@]$(X)

$(NAME): $(OBJ)
	@echo $(Y)Compiling [$(SRC)]
	@echo into [$(NAME)]...$(X)
	@c++ $(CFLAGS) $(OBJ) -o $(NAME)
	@echo $(G)Finished [$(NAME)]$(X)

clean:
	@if [ -d "$(OBJ_PATH)" ]; then \
			rm -f -r $(OBJ_PATH); \
			echo $(R)Cleaning" "[$(OBJ) $(OBJ_PATH)]...$(X); else \
			echo "No objects to remove."; \
	fi;

fclean: clean
	@if [ -f "$(NAME)" ]; then \
			rm -f $(NAME); \
			echo $(R)Cleaning" "[$(NAME)]...$(X);else \
			echo "No executable to remove."; \
	fi;

re: fclean all

# ADDITIONAL RULES

.PHONY: all, clean, fclean, re
