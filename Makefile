NAME = ft_nm

##### SRC & OBJ PATH #####
SRCPATH = ./srcs
OBJPATH = $(SRCPATH)/obj

##### LIB #####
LIBFTPATH= ./libft
LIBFT= -L $(LIBFTPATH) -lft

##### INCLUDE #####
PATH_INCLUDE = $(SRCPATH)/includes
PATH_INCLUDE2 = ./libft
HEADERS = $(PATH_INCLUDE)/*.h
HEADERS += $(PATH_INCLUDE2)/*.h
INC = $(addprefix -I , $(PATH_INCLUDE) $(PATH_INCLUDE2))


##### COMPILER #####
CC = clang
##### COMPILATION FLAG #####
CCFLAGS = -Wall -Wextra -Werror

##### OSTYPE #####
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	# mac
	CCFLAGS += -D DARWIN
else
	#Linux and others...
	CCFLAGS += -D LINUX
endif

##### SRCS #####
SRCS = $(addprefix $(SRCPATH)/, ft_nm.c)

OBJ = $(SRCS:$(SRCPATH)/%.c=$(OBJPATH)/%.o)


### RULES ###

all: mk_objdir $(NAME)


mk_objdir:
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi


$(NAME): $(LIBFT) $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBFT)

$(LIBFT): 
	@make -C $(LIBFTPATH)

$(OBJPATH)/%.o: $(SRCPATH)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

### CLEAN ###
.PHONY: sanitize clean fclean re

clean:
	@make clean -C $(LIBFTPATH)
	rm -rf $(OBJ)

fclean: clean
	@make fclean -C $(LIBFTPATH)
	rm -f $(NAME)

re: fclean
	@$(MAKE)

