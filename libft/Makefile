# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amineau <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/11/23 15:32:27 by amineau           #+#    #+#              #
#    Updated: 2016/03/22 14:03:09 by amineau          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libprt.a
CC = gcc 
FLAGS = -Wall -Werror -Wextra

SRCS = ft_printf.c ft_stock.c ft_list.c ft_algo.c ft_straddc.c ft_itoa_double.c\
		ft_itoa_scien.c ft_itoa_fonc.c ft_arrondi.c ft_nbrdigit.c ft_putwint.c \
		ft_putwchar.c ft_cntwint.c ft_libft_wchar.c ft_atof.c ft_wildcard.c \
		ft_recup_arg.c ft_justif.c ft_zero_justif.c ft_integer.c ft_floater.c \
		ft_wchar.c

SRCS += ft_putchar.c ft_putstr.c ft_putendl.c ft_putnbr.c ft_strlen.c \
		ft_strdup.c ft_strcpy.c ft_strncpy.c ft_strcat.c ft_strncat.c \
		ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strstr.c ft_strnstr.c \
		ft_strcmp.c ft_strncmp.c ft_strclr.c ft_memset.c ft_memcpy.c \
		ft_memchr.c ft_memccpy.c ft_memmove.c ft_memcmp.c ft_bzero.c \
		ft_isupper.c ft_islower.c ft_isalpha.c ft_isdigit.c ft_isalnum.c \
		ft_isascii.c ft_isprint.c ft_toupper.c ft_tolower.c ft_atoi.c \
		ft_strnew.c ft_memalloc.c ft_itoa.c ft_memdel.c ft_strdel.c ft_strequ.c\
		ft_strnequ.c ft_striter.c ft_striteri.c ft_strmap.c ft_strmapi.c \
		ft_strsub.c ft_strjoin.c ft_strtrim.c ft_strsplit.c ft_putchar_fd.c \
		ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_lstnew.c \
		ft_lstdelone.c ft_lstdel.c ft_lstadd.c ft_lstiter.c ft_lstmap.c \
		ft_min.c ft_max.c ft_putnbrendl.c ft_strclnjoin.c ft_strndup.c \
		get_next_line.c ft_abs.c ft_strcasecmp.c ft_strnchr.c ft_itoa_base.c \
		ft_power.c ft_strcln1join.c ft_itoa_unsi.c ft_itoa_unsi_base.c \
		ft_strcln2join.c ft_atoi_base.c ft_atol.c

SRCDIR = srcs
OBJDIR = objs

SRC = $(addprefix $(SRCDIR)/,$(SRCS))
OBJ = $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
INC = includes

WHITE	= \033[0m
DARK	= \033[1;30m
RED		= \033[1;31m
GREENB	= \033[1;32m
GREEN	= \033[0;32m
YELLOW	= \033[33m
CYAN	= \033[36m

all: $(NAME)

$(NAME): $(OBJ)
	@printf "$(YELLOW)%-30s$(WHITE)" "Building $@"
	@ar rc $@ $^ && echo "$(GREENB)<<--$(WHITE)" \
	|| echo "$(RED)<<--$(WHITE)"
	@ranlib $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(FLAGS) -I$(INC) -o $@ -c $^ \
	&& printf "%-30s$(DARK)-->>\t$(GREEN)$@$(WHITE)\n" "$<" \
	|| (printf "%-30s$(DARK)-->>\t$(RED)$@$(WHITE)\n" "$<" \
	&& exit 1)

clean:
	@printf "$(YELLOW)%-30s$(WHITE)" "Deleting $(OBJDIR)"
	@rm -rf $(OBJDIR)
	@echo "$(GREENB)<<--$(WHITE)"

fclean: clean
	@printf "$(YELLOW)%-30s$(WHITE)" "Deleting $(NAME)"
	@rm -f $(NAME)
	@echo "$(GREENB)<<--$(WHITE)"

re: fclean all
