SERVER_NAME = server
CLIENT_NAME = client

CC = gcc

SERVER_SRCS = server/main.c server/cmd_generic.c server/cmd_auth.c server/response_to_client.c \
			  server/get_dir.c server/cmd_files_management.c server/cmd_transfert.c common/openssl.c \
			  common/read_sock.c
# ftp_action_2.c ftp_action_3.c \
			  ftp_action_4.c
CLIENT_SRCS = client/main.c common/openssl.c common/read_sock.c


INCS = ft_p.h

LFTPATH  = libft
SPATH    = srcs
OPATH    = objs
HPATH    = includes
LFTHPATH = $(LFTPATH)/includes

CFLAGS = -Wall -Wextra -g # TODO : Adding -Werror
IFLAGS = -I./$(HPATH) -I./$(LFTHPATH)
LIBS   = -L./$(LFTPATH) -lprt -lpam -lpam_misc -lssl -lcrypto


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
		@mkdir -p $(shell dirname $@)
		@$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $< \
		&& printf "%-30s$(DARK)-->>\t$(GREEN)$@$(WHITE)\n" "$<" \
		|| (printf "%-30s$(DARK)-->>\t$(RED)$@$(WHITE)\n" "$<" \
		&& exit 1)

generate_ssl:
		# openssl req -x509 -newkey rsa:4096 -keyout server.key -out server.csr \
		# 	-days 365 -subj "/C=FR/ST=IDF/L=Paris/O=42/CN=ft_p.42" -nodes
		openssl req -new -newkey rsa:2048 -nodes -keyout server.key -out server.csr -subj "/CN=localhost"
		openssl x509 -req -in server.csr -signkey server.key -out server.crt

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
