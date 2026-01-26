# === Variables ===

# The name of the executable
NAME = RPN

# The compiler
CC = c++

# Compiler flags as per the subject requirements
CFLAGS = -Wall -Wextra -Werror -std=c++98

# === Directories ===

# Source files are in the current directory
SRC_DIR = .

# Object files will be placed here
OBJ_DIR = obj

# === Files ===

# List of all source (.cpp) files
SRCS = main.cpp RPN.cpp


# List of all header (.hpp) files for dependency tracking
HEADERS = RPN.hpp \


# Generate the list of object files from the source files
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# === Rules ===

# Default target.
# It depends on the $(OBJ_DIR) being created first, then on the executable.
all: $(OBJ_DIR) $(NAME)

# Rule to create the executable by linking the object files
$(NAME): $(OBJS)
	@echo "Linking object files to create $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(NAME) created successfully."

# Rule to create the object directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Pattern rule to compile a .cpp file into a .o file
# A target like 'obj/main.o' will depend on 'main.cpp' and all headers.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Removes only the object files
clean:
	@rm -rf $(OBJ_DIR)
	@echo "Object files removed."

# Removes object files and the final executable
fclean: clean
	@rm -f $(NAME)
	@echo "Executable removed."

# Forces a full rebuild of the project
re: fclean all

# Declare targets that are not files
.PHONY: all clean fclean re
