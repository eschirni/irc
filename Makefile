# FLAGS
CPPFLAGS = -Wall -Wextra -std=c++98 -Wshadow -Wno-shadow -g3 -fsanitize=address #-Werror
CXX = @c++

# COLORS
N = \033[30m
M = \033[36m
R = \033[31m
G = \033[32m
B = \033[34m
X = \033[0m

# EXECUTABLE
NAME = ./ircserv

# PATHS
SRC_PATH = ./src/
OBJ_PATH = ./obj/

# SOURCES
SRC = $(shell find . -iname "*.cpp")

# OBJECTS
OBJ = $(patsubst $(SRC_PATH)%.c, $(OBJ_PATH)%.o, $(SRC))

# RULES
all: $(NAME)

$(OBJ_PATH)%.o :$(SRC_PATH)%.c
	@echo "$(N)Compiling [$@]...$(X)"
	@mkdir -p $(dir $@)
	@$(CXX) $(CPPFLAGS) -c -o $@ $<
	@echo "$(G)Finished [$@]$(X)"

$(NAME): $(OBJ)
	@$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)
	@echo "$(M)Finished [$(NAME)]$(X)"

clean:
	@if [ -d "$(OBJ_PATH)" ]; then \
			rm -rf $(OBJ_PATH); \
			echo "$(R)Cleaning [$(OBJ) $(OBJ_PATH)]...$(X)"; else \
			echo "$(N)No objects to remove."; \
	fi;

fclean: clean
	@if [ -f "$(NAME)" ]; then \
			rm -f $(NAME); \
			echo "$(R)Cleaning [$(NAME)]...$(X)";else \
			echo "No executable to remove."; \
	fi;

re: fclean all
