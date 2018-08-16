SERVER_NAME = server
CLIENT_NAME = client

CC = gcc

SERVER_SRCS = server.c ftp_action_1.c # ftp_action_2.c ftp_action_3.c \
			  ftp_action_4.c
CLIENT_SRCS = client.c


INCS = ft_p.h

LFTPATH  = libft
SPATH    = srcs
OPATH    = objs
HPATH    = includes
LFTHPATH = $(LFTPATH)/includes

CFLAGS = -Wall -Wextra -g # TODO : Adding -Werror
IFLAGS = -I./$(HPATH) -I./$(LFTHPATH)
LIBS   = -L./$(LFTPATH) -lprt


INC = $(addprefix $(HPATH)/,$(INCS))
SRC = $(addprefix $(SPATH)/,$(SRCS))
SERVER_OBJ = $(addprefix $(OPATH)/,$(SERVER_SRCS:.c=.o))
CLIENT_OBJ = $(addprefix $(OPATH)/,$(CLIENT_SRCS:.c=.o))

WHITE   = \033[0m
DARK    = \033[1;30m
RED     = \033[1;31m
GREENB  = \033[1;32m
GREEN   = \033[0;32m
YELLOW  = \033[33m
CYAN    = \033[36m

all: $(OPATH) $(INC) $(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER_OBJ)
		@echo "$(YELLOW)Compilation Libft$(WHITE)"
		@make -C $(LFTPATH)
		@$(CC) -o $@ $^ $(LIBS) \
		&& printf "$(YELLOW)%-30s$(DARK)-->>\t$(GREEN)$@$(WHITE)\n" "Compilation"\
		|| (printf "$(YELLOW)%-30s$(DARK)-->>\t$(RED)$@$(WHITE)\n" "Compilation" \
		&& exit 1)


$(CLIENT_NAME): $(CLIENT_OBJ)
		@echo "$(YELLOW)Compilation Libft$(WHITE)"
		@make -C $(LFTPATH)
		@$(CC) -o $@ $^ $(LIBS) \
		&& printf "$(YELLOW)%-30s$(DARK)-->>\t$(GREEN)$@$(WHITE)\n" "Compilation"\
		|| (printf "$(YELLOW)%-30s$(DARK)-->>\t$(RED)$@$(WHITE)\n" "Compilation" \
		&& exit 1)

$(OPATH):
		@printf "$(YELLOW)%-30s$(WHITE)" "Creating $@ directory"
		@mkdir -p $(OPATH)
		@echo "$(GREENB)<<--$(WHITE)"

$(OPATH)/%.o: $(SPATH)/%.c
		@$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $< \
		&& printf "%-30s$(DARK)-->>\t$(GREEN)$@$(WHITE)\n" "$<" \
		|| (printf "%-30s$(DARK)-->>\t$(RED)$@$(WHITE)\n" "$<" \
		&& exit 1)

clean:
		@printf "$(YELLOW)%-30s$(WHITE)" "Deleting $(OPATH)"
		@rm -rf $(OPATH)
		@echo "$(GREENB)<<--$(WHITE)"

fclean: clean
		@printf "$(YELLOW)%-30s$(WHITE)" "Deleting $(SERVER_NAME) and $(CLIENT_NAME)"
		@rm -f $(SERVER_NAME) $(CLIENT_NAME)
		@echo "$(GREENB)<<--$(WHITE)"

libclean: fclean
		@make -C $(LFTPATH) fclean

re: fclean all
