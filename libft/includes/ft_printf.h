/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/25 13:15:08 by amineau           #+#    #+#             */
/*   Updated: 2016/03/21 16:46:30 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <wchar.h>
# include <stdint.h>
# include "libft.h"

# define TYPE "sSpdDioOfFuUxXcCbBeE"
# define ABS(x) (((x) < 0) ? -(x) : (x))

typedef struct	s_format
{
	char			just;
	char			sign;
	char			conv;
	int				width;
	int				wild_width;
	int				precision;
	char			*lenght;
	char			type;
	size_t			size;
	struct s_format	*next;
}				t_format;

int				ft_printf(const char *format, ...);
int				ft_algo(t_format *lst, char *str, va_list ap);
int				ft_listadd_format(t_format *list, t_format **begin);
int				ft_nbrdigit(uintmax_t nb);
int				ft_cntwint(unsigned int w);
int				ft_putwchar(wchar_t *w, t_format *lst);
int				ft_arrondi(long double nb);
int				ft_char(t_format *lst, va_list ap);
int				ft_string(t_format *lst, va_list ap);
int				ft_int(t_format *lst, va_list ap);
int				ft_unint(t_format *lst, va_list ap);
int				ft_adrpoint(t_format *lst, va_list ap);
int				ft_float(t_format *lst, va_list ap);
int				ft_scienti(t_format *lst, va_list ap);
int				ft_octal(t_format *lst, va_list ap);
int				ft_binaire(t_format *lst, va_list ap);
int				ft_hexa(t_format *lst, va_list ap);
int				ft_wint(t_format *lst, va_list ap);
int				ft_wchar(t_format *lst, va_list ap);
unsigned int	ft_wcharlen(wchar_t *w);
long double		ft_arrondi_double(long double nb, size_t dec);
long double		ft_atof(char *str);
char			*ft_itoa_double(double nb, int prec, char c);
char			*ft_itoa_scien(double nb, int prec, char c);
char			*stock_flag(t_format *lst, char *str);
char			*stock_width(int *width, int *wild, char *pourc);
char			*stock_precision(int *prec, char *pourc, int width);
char			*stock_lenght(char **str, char *pourc);
char			*ft_straddc(char *str, char c);
char			*ft_wh(char c, int n);
char			*ft_decimal(long double nb, int prec);
char			*ft_exposant(char e, int dec);
char			*ft_nan_or_inf(double nb, char c);
char			*ft_decimal(long double nb, int prec);
char			*ft_integer(long double nb, char *str, int nbrdig, int prec);
char			*ft_exposant(char e, int dec);
char			*ft_signed_size(t_format *lst, int base, va_list ap);
char			*ft_unsigned_size(t_format *lst, int base, va_list ap);
char			*ft_precision(char *str, int prec);
char			*ft_justif(char *str, int just, char f);
char			*ft_justif_string(char *str, size_t just, char f);
char			*ft_sign(char *str, char s);
char			*ft_lenght_type(char c, char **str);
char			*ft_zero_justif(char *str, t_format *lst, char *dest);
wchar_t			*ft_wcharndup(wchar_t *s1, size_t n);
void			ft_refresh(char **str);
void			ft_putwint(unsigned int w, int oc);
void			ft_refresh(char **str);
void			ft_lstdel_format(t_format *lst);
void			ft_close_lst(t_format **lst, va_list ap);
void			ft_wildcard(t_format *lst, va_list ap);
t_format		*ft_listnew_format(char *pourc);

#endif
