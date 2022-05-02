SERVER_NAME = server
CLIENT_NAME = client

CC = gcc

SERVER_SRCS = server/main.c server/cmd_generic.c server/cmd_auth.c \
			  server/responses.c server/path_utils.c common/utils.c \
			  server/cmd_files_management.c server/cmd_transfert.c \
			  common/openssl.c common/read_sock.c common/socket.c \
			  common/error.c server/close.c common/addr_utils.c
# ftp_action_2.c ftp_action_3.c \
			  ftp_action_4.c
CLIENT_SRCS = client/main.c common/openssl.c \
			  common/read_sock.c common/utils.c client/cmd_auth.c \
			  client/cmd_files_management.c client/responses.c \
			  client/cmd_general.c client/lexer.c client/protocol.c \
			  client/cmd_transfert.c common/socket.c common/error.c \
			  client/cmd_local.c common/addr_utils.c

DEBUGGER := gdb

INCS = ft_p.h ftp_server.h ftp_client.h

LFTPATH  = libft
SPATH    = srcs
OPATH    = objs
HPATH    = includes
LFTHPATH = $(LFTPATH)/includes

CFLAGS = -Wall -Wextra -Werror -g
IFLAGS = -I./$(HPATH) -I./$(LFTHPATH)
CLIENT_LIBS = -L./$(LFTPATH) -lprt -lssl -lcrypto
SERVER_LIBS = -L./$(LFTPATH) -lprt -lpam -lpam_misc -lssl -lcrypto

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

.PHONY: all

all: $(OPATH) $(INC) $(SERVER_NAME) $(CLIENT_NAME)
	@:

$(SERVER_NAME): $(SERVER_OBJ)
		@echo "$(YELLOW)Compilation Libft$(WHITE)"
		@make -C $(LFTPATH)
		@$(CC) -o $@ $^ $(SERVER_LIBS) \
		&& printf "$(YELLOW)%-30s$(DARK)-->>\t$(GREEN)$@$(WHITE)\n" "Compilation"\
		|| (printf "$(YELLOW)%-30s$(DARK)-->>\t$(RED)$@$(WHITE)\n" "Compilation" \
		&& exit 1)


$(CLIENT_NAME): $(CLIENT_OBJ)
		@echo "$(YELLOW)Compilation Libft$(WHITE)"
		@make -C $(LFTPATH)
		@$(CC) -o $@ $^ $(CLIENT_LIBS) \
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

debug_server: $(OPATH) $(SERVER_NAME) $(INC)
	$(DEBUGGER) $(SERVER_NAME)

debug_client: $(OPATH) $(CLIENT_NAME) $(INC)
	$(DEBUGGER) $(CLIENT_NAME)

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
