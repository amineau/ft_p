/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/23 13:21:26 by amineau           #+#    #+#             */
/*   Updated: 2022/04/21 23:35:43 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
#define LIBFT_H

#include "ft_printf.h"
#include <fcntl.h>
#include <openssl/ssl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 1024

typedef struct s_list
{
	void          *content;
	size_t         content_size;
	struct s_list *next;
} t_list;

typedef struct s_fd_remainder
{
	int    fd;
	char **remainder;
} t_fd_remainder;

typedef struct s_ssl_remainder
{
	SSL   *ssl;
	char **remainder;
} t_ssl_remainder;

void      ft_putchar(char c);
void      ft_putstr(char const *s);
void      ft_putendl(char const *s);
void      ft_putnbr(int n);
void      ft_putnbrendl(int n);
void      ft_putchar_fd(int fd, char c);
void      ft_putstr_fd(int fd, char const *s);
void      ft_putendl_fd(int fd, char const *s);
void      ft_putnbr_fd(int fd, int n);
size_t    ft_strlen(const char *s);
char	 *ft_strdup(const char *s1);
char	 *ft_strndup(const char *s1, size_t n);
char	 *ft_strcpy(char *dst, const char *src);
char	 *ft_strncpy(char *dst, const char *src, size_t n);
char	 *ft_strcat(char *s1, const char *s2);
char	 *ft_strncat(char *s1, const char *s2, size_t n);
size_t    ft_strlcat(char *dst, const char *src, size_t n);
char	 *ft_strchr(const char *s, int c);
char	 *ft_strnchr(const char *s, int c, int n);
char	 *ft_strrchr(const char *s, int c);
char	 *ft_strstr(const char *s1, const char *s2);
char	 *ft_strnstr(const char *s1, const char *s2, size_t n);
int       ft_arraystr(const char **array, const char *str);
char	 *ft_arrayjoin(char **array);
char	 *ft_arraycjoin(char **array, char c);
void      ft_arrayfree(const char **array);
int       ft_strcmp(const char *s1, const char *s2);
int       ft_strcasecmp(const char *s1, const char *s2);
int       ft_strncasecmp(const char *s1, const char *s2, size_t n);
int       ft_strncmp(const char *s1, const char *s2, size_t n);
void	 *ft_memset(void *b, int c, size_t len);
void	 *ft_memcpy(void *dst, const void *src, size_t n);
void	 *ft_memchr(const void *s, int c, size_t n);
void	 *ft_memccpy(void *dst, const void *src, int c, size_t n);
void	 *ft_memmove(void *dst, const void *src, size_t len);
int       ft_memcmp(const void *s1, const void *s2, size_t n);
void      ft_bzero(void *s, size_t n);
int       ft_max(int *tab, size_t len);
int       ft_min(int *tab, size_t len);
uintmax_t ft_power(int nbr, int base);
intmax_t  ft_abs(intmax_t n);
int       ft_isupper(int c);
int       ft_islower(int c);
int       ft_isalpha(int c);
int       ft_isdigit(int c);
int       ft_isnumber(const char *);
int       ft_isalnum(int c);
int       ft_isascii(int c);
int       ft_isprint(int c);
int       ft_toupper(int c);
int       ft_tolower(int c);
int       ft_atoi(const char *str);
long      ft_atol(const char *str);
int       ft_atoi_base(const char *str, int base);
void      ft_strclr(char *s);
char	 *ft_strnew(size_t size);
char	 *ft_itoa(intmax_t n);
char	 *ft_utoa(uintmax_t n);
char	 *ft_itoa_base(intmax_t n, int base, char c);
char	 *ft_utoa_base(uintmax_t n, int base, char c);
void	 *ft_memalloc(size_t size);
void      ft_memdel(void **ap);
void      ft_strdel(char **as);
int       ft_strequ(char const *s1, char const *s2);
int       ft_strnequ(char const *s1, char const *s2, size_t n);
void      ft_striter(char *s, void (*f)(char *));
void      ft_striteri(char *s, void (*f)(unsigned int, char *));
char	 *ft_strmap(char const *s, char (*f)(char));
char	 *ft_strmapi(char const *s, char (*f)(unsigned int, char));
char	 *ft_strsub(char const *s, unsigned int start, size_t len);
char	 *ft_strcjoin(char const *s1, char const *s2, char c);
char	 *ft_strjoin(char const *s1, char const *s2);
char	 *ft_strclnjoin(char const *s1, char const *s2);
char	 *ft_strcln1join(char const *s1, char const *s2);
char	 *ft_strcln2join(char const *s1, char const *s2);
char	 *ft_strtrim(char const *s);
char	 *ft_strrtrim(char const *s);
char    **ft_strsplit(char const *s, char c);
t_list   *ft_lstnew(void const *content, size_t content_size);
void      ft_lstdelone(t_list **alst, void (*del)(void *, size_t));
void      ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void      ft_lstadd(t_list **alst, t_list *new);
void      ft_lstaddend(t_list **alst, t_list *new);
void      ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list   *ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));
t_list   *ft_lstfindfirst(t_list     *lst,
						  void const *content,
						  int (*f)(t_list *, void const *));
void      ft_lstremoveelem(t_list **alst, t_list *elem);
int       get_next_line(int fd, char **line);
int       get_next_line_eol(int fd, char **line, const char *eol);
int       get_next_line_ssl(SSL *ssl, char **line);
int       get_next_line_ssl_eol(SSL *ssl, char **line, const char *eol);
int       ft_printf(const char *format, ...);

#endif
