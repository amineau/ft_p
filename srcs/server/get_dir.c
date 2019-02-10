/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 05:56:41 by amineau           #+#    #+#             */
/*   Updated: 2019/02/10 04:50:00 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

char*	ft_getcwd(void)
{
	size_t	len;
	char*	cwd;
	char*	str;

	len = 50;
	if ((str = (char*)malloc(sizeof(char) * len)) == NULL)
		exit(-1);
	errno = 0;
	cwd = getcwd(str, len);
	// ft_putendl("ft_getcwd malloc pass");
	// while ((cwd = getcwd(str, len)) == NULL && errno == ERANGE)
	// {
	// 	ft_putendl("ft_getcwd begin while");		
	// 	len += BUFF_SIZE;
	// 	// TODO : ft_realloc
	// 	str = realloc(str, len);
	// 	ft_putendl("ft_getcwd end while");		
	// }
	// ft_putendl(cwd);
	// ft_putendl(str);
	// ft_putendl(strerror(errno));
	// ft_putendl("ft_getcwd end function");
	return (str);
}

char	*get_root(void)
{
	static char	*root;

	if (!root)
		root = ft_getcwd();
	return (root);
}

char	*get_wdir(void)
{
	char*	wdir;
	char*	cwd;
	char*	root;
	size_t	root_len;
	size_t	cwd_len;

	cwd = ft_getcwd();
	root = get_root();
	root_len = ft_strlen(root);
	cwd_len = ft_strlen(cwd);
	if (ft_strcmp(root, cwd))
		wdir = ft_strsub(cwd, root_len, cwd_len - root_len);
	else
		wdir = ft_strdup("/");
	ft_strdel(&cwd);
	return (wdir);
}