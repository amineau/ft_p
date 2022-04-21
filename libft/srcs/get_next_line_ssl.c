/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_ssl.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/04 10:54:48 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 23:35:53 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char *ft_get_next_line(char *buff, const char *eol)
{
	int i;

	i = 0;
	while (buff[i])
	{
		if (!ft_strncmp(&(buff[i]), eol, ft_strlen(eol)))
			return (&(buff[i]));
		i++;
	}
	return (NULL);
}

static int ft_is_end_of_file(char *line, int size)
{
	return (size == 0 && (!line || line[0] == '\0'));
}

static int ft_get_line_in_remainder(char **remainder, char **line, const char *eol)
{
	char  *p;
	size_t eol_size;

	eol_size = ft_strlen(eol);
	if (*remainder)
	{
		if ((p = ft_strstr(*remainder, eol)))
		{
			*line = ft_strndup(*remainder, p - *remainder);
			ft_memmove(*remainder, p + eol_size, ft_strlen(p + eol_size) + 1);
			return (1);
		}
		else
		{
			*line = ft_strdup(*remainder);
			ft_strclr(*remainder);
		}
	}
	return (0);
}

static char *ft_store(char **remainder, char *next_line, const char *eol)
{
	int    i;
	size_t len_eol;
	size_t len_remainder;

	i = 0;
	ft_strdel(remainder);
	if (!next_line)
		return (NULL);
	len_eol = ft_strlen(eol);
	len_remainder = ft_strlen(next_line) - i;
	ft_bzero(next_line, len_eol);
	if (!(*remainder = ft_memalloc(sizeof(char) * len_remainder + 1)))
		return (NULL);
	while ((next_line)[len_eol + i])
	{
		(*remainder)[i] = (next_line)[len_eol + i];
		i++;
	}
	(*remainder)[i] = '\0';
	return (*remainder);
}

static t_ssl_remainder *build_remainder_content(SSL *ssl, char **remainder)
{
	t_ssl_remainder *ssl_remainder;

	ssl_remainder = (t_ssl_remainder *)malloc(sizeof(t_ssl_remainder));
	ssl_remainder->ssl = ssl;
	ssl_remainder->remainder = remainder;
	*ssl_remainder->remainder = NULL;
	return ssl_remainder;
}

static int is_ssl_elem(t_list *elem, const void *ssl)
{
	return (elem->content && ((t_ssl_remainder *)elem->content)->ssl == *(SSL **)ssl);
}

static t_list **get_static_list()
{
	static t_list **alst = NULL;

	if (!alst)
	{
		alst = (t_list **)malloc(sizeof(t_list *));
		*alst = NULL;
	}
	return alst;
}

static char **get_remained(SSL *ssl)
{
	t_list         **alst;
	t_list          *elem;
	t_ssl_remainder *content;
	char           **remainder;

	alst = get_static_list();

	elem = ft_lstfindfirst(*alst, &ssl, &is_ssl_elem);
	if (!elem)
	{
		remainder = (char **)malloc(sizeof(char *));
		content = build_remainder_content(ssl, remainder);
		elem = ft_lstnew(content, sizeof(t_ssl_remainder));
		if (!*alst)
		{
			*alst = elem;
		}
		else
			ft_lstaddend(alst, elem);
	}
	return (((t_ssl_remainder *)elem->content)->remainder);
}

static void clean_remained(SSL *ssl)
{
	t_list **alst;
	t_list  *elem;

	alst = get_static_list();

	elem = ft_lstfindfirst(*alst, &ssl, &is_ssl_elem);

	ft_lstremoveelem(alst, elem);
	ft_strdel(((t_ssl_remainder *)elem->content)->remainder);
	ft_memdel((void **)&elem->content);
	ft_memdel((void **)&elem);
}

int get_next_line_ssl_eol(SSL *ssl, char **line, const char *eol)
{
	char  *buff;
	char **remainder;
	int    size;
	char  *next_line;
	int    return_code;

	remainder = get_remained(ssl);

	if (!ssl || line == NULL)
		return (-1);
	*line = NULL;
	if (ft_get_line_in_remainder(remainder, line, eol))
		return (1);
	return_code = -1;
	while (return_code == -1)
	{
		buff = ft_memalloc(sizeof(char) * (BUFF_SIZE + 1));
		if ((size = SSL_read(ssl, buff, BUFF_SIZE)) == -1)
			return (-1);
		buff[size] = '\0';
		next_line = ft_get_next_line(buff, eol);

		*remainder = ft_store(remainder, next_line, eol);

		if (ft_is_end_of_file(*line, size))
			return_code = 0;
		else
			*line = ft_strclnjoin(*line, buff);
		if (return_code && (!size || *remainder))
			return_code = 1;
	}
	if (!return_code)
		clean_remained(ssl);
	return (return_code);
}

int get_next_line_ssl(SSL *ssl, char **line)
{
	return get_next_line_ssl_eol(ssl, line, "\n");
}