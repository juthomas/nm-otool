NAME = ft_nm
CC = gcc

COLOR = \0033[1;35m

SRC_PATH = ./src/
OBJ_PATH = ./obj/
LIB_PATH = ./lib/
INC_PATH = ./include/ $(LIB_PATH)libft/includes/ $(LIB_PATH)glfw/include/ \

GCC_FLGS = -g #-Werror -Wextra -Wall
# GCC_LIBS = -lglfw3 -framework AppKit -framework OpenGL -framework IOKit -framework CoreVideo 

SRC_NAME = main.c

OBJ_NAME = $(SRC_NAME:.c=.o)
LIB_NAME = libft

SRC = $(addprefix $(SRC_PATH), $(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))
INC = $(addprefix -I,$(INC_PATH))
LIB = $(addprefix -L$(LIB_PATH),$(LIB_NAME))

all: $(NAME)
	@echo "$(COLOR)$(NAME) \033[100D\033[40C\0033[1;30m[All OK]\0033[1;37m"


$(NAME): $(OBJ)
	@make -C $(LIB_PATH)libft -j --no-print-directory
	@echo "$(COLOR)Objects \033[100D\033[40C\0033[1;32m[Created]\0033[1;37m"
	@# make -C $(LIB_PATH)libmat -j --no-print-directory
	@$(CC) $(GCC_FLGS) $(LIB) -lft $(INC) $(OBJ) -o $(NAME)
	@echo "$(COLOR)$(NAME) \033[100D\033[40C\0033[1;32m[Created]\0033[1;37m"


$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(GCC_FLGS) $(INC) -o $@ -c $<
	@#echo "$(COLOR)Compiled :\033[100D\033[40C\0033[1;32m$(@:$(OBJ_PATH)/%=%)\0033[1;37m"
	@echo "$(COLOR)$(@:$(OBJ_PATH)/%=%) \033[100D\033[40C\0033[1;32m[Compiled]\0033[1;37m"
	

clean:
	@rm -f $(OBJ)
	@rm -rf $(OBJ_PATH)
	@echo "$(COLOR)Objects \033[100D\033[40C\0033[1;31m[Deleted]\0033[1;37m"


fclean: clean
	@make -C $(LIB_PATH)libft fclean --no-print-directory
	@echo "$(COLOR)$(LIB_PATH)libft \033[100D\033[40C\0033[1;31m[Cleaned]\0033[1;37m"
	@# make -C $(LIB_PATH)libmat fclean --no-print-directory
	@rm -f $(NAME)
	@echo "$(COLOR)$(NAME) \033[100D\033[40C\0033[1;31m[Deleted]\0033[1;37m"


re: fclean all

coffee: all
	@echo ""
	@echo "         {"
	@echo "      {   }"
	@echo "       }\0033[1;34m_\0033[1;37m{ \0033[1;34m__\0033[1;37m{"
	@echo "    \0033[1;34m.-\0033[1;37m{   }   }\0033[1;34m-."
	@echo "   \0033[1;34m(   \0033[1;37m}     {   \0033[1;34m)"
	@echo "   \0033[1;34m| -.._____..- |"
	@echo "   |             ;--."
	@echo "   |            (__  \ "
	@echo "   |             | )  )"
	@echo "   |   \0033[1;96mCOFFEE \0033[1;34m   |/  / "
	@echo "   |             /  / "
	@echo "   |            (  / "
	@echo "   \             | "
	@echo "     -.._____..- "
	@echo ""
	@echo ""
	@echo "\0033[1;32m\033[3CTake Your Coffee"
	@echo "\0033[1;37m"
	@./$(NAME)