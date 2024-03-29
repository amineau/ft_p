/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/04 10:54:48 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 23:22:28 by amineau          ###   ########.fr       */
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

static t_fd_remainder *build_remainder_content(int fd, char **remainder)
{
	t_fd_remainder *fd_remainder;

	fd_remainder = (t_fd_remainder *)malloc(sizeof(t_fd_remainder));
	fd_remainder->fd = fd;
	fd_remainder->remainder = remainder;
	*fd_remainder->remainder = NULL;
	return fd_remainder;
}

static int is_fd_elem(t_list *elem, const void *fd)
{
	return (elem->content && ((t_fd_remainder *)elem->content)->fd == *(int *)fd);
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

static char **get_remained(const int fd)
{
	t_list        **alst;
	t_list         *elem;
	t_fd_remainder *content;
	char          **remainder;

	alst = get_static_list();

	elem = ft_lstfindfirst(*alst, &fd, &is_fd_elem);
	if (!elem)
	{
		remainder = (char **)malloc(sizeof(char *));
		content = build_remainder_content(fd, remainder);
		elem = ft_lstnew(content, sizeof(t_fd_remainder));
		if (!*alst)
		{
			*alst = elem;
		}
		else
			ft_lstaddend(alst, elem);
	}
	return (((t_fd_remainder *)elem->content)->remainder);
}

static void clean_remained(const int fd)
{
	t_list **alst;
	t_list  *elem;

	alst = get_static_list();

	elem = ft_lstfindfirst(*alst, &fd, &is_fd_elem);

	ft_lstremoveelem(alst, elem);
	ft_strdel(((t_fd_remainder *)elem->content)->remainder);
	ft_memdel((void **)&elem->content);
	ft_memdel((void **)&elem);
}

int get_next_line_eol(const int fd, char **line, const char *eol)
{
	char  *buff;
	char **remainder;
	int    size;
	char  *next_line;
	int    return_code;

	remainder = get_remained(fd);

	if (fd < 0 || line == NULL)
		return (-1);
	*line = NULL;
	if (ft_get_line_in_remainder(remainder, line, eol))
		return (1);
	return_code = -1;
	while (return_code == -1)
	{
		buff = ft_memalloc(sizeof(char) * (BUFF_SIZE + 1));
		if ((size = read(fd, buff, BUFF_SIZE)) == -1)
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
		clean_remained(fd);
	return (return_code);
}

int get_next_line(const int fd, char **line)
{
	return get_next_line_eol(fd, line, "\n");
}