# FLAGS
CPPFLAGS = -Wall -Wextra -std=c++98 -Wshadow -Wno-shadow #-g #-Werror
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
SRC_PATH = ./
OBJ_PATH = ./obj/

# SOURCES
SRC = $(shell find . -iname "*.cpp")

# OBJECTS
OBJ = $(patsubst $(SRC_PATH)%.c, $(OBJ_PATH)%.o, $(SRC))

# RULES
all: $(NAME)
	# @echo "  $(R)iiii                                          "; sleep .2
	# @echo " i::::i                                         "; sleep .3
	# @echo "  iiii                                          \n"; sleep .3
	# @echo "iiiiiii $(G)rrrrr   rrrrrrrrr        $(B)cccccccccccccccc"; sleep .3
	# @echo "$(R)i:::::i $(G)r::::rrr:::::::::r     $(B)cc:::::::::::::::c"; sleep .3
	# @echo " $(R)i::::i $(G)r:::::::::::::::::r   $(B)c:::::::::::::::::c"; sleep .3
	# @echo " $(R)i::::i $(G)rr::::::rrrrr::::::r $(B)c:::::::cccccc:::::c"; sleep .3
	# @echo " $(R)i::::i  $(G)r:::::r     r:::::r $(B)c::::::c     ccccccc"; sleep .3
	# @echo " $(R)i::::i  $(G)r:::::r     rrrrrrr $(B)c:::::c             "; sleep .3
	# @echo " $(R)i::::i  $(G)r:::::r             $(B)c:::::c             "; sleep .3
	# @echo " $(R)i::::i  $(G)r:::::r             $(B)c::::::c     ccccccc"; sleep .3
	# @echo "$(R)i::::::i $(G)r:::::r             $(B)c:::::::cccccc:::::c"; sleep .3
	# @echo "$(R)i::::::i $(G)r:::::r              $(B)c:::::::::::::::::c"; sleep .3
	# @echo "$(R)i::::::i $(G)r:::::r               $(B)cc:::::::::::::::c"; sleep .3
	# @echo "$(R)iiiiiiii $(G)rrrrrrr                 $(B)cccccccccccccccc$(X)"

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
			rm -f -r $(OBJ_PATH); \
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

# ADDITIONAL RULES

.PHONY: all, clean, fclean, re
