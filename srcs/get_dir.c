/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 05:56:41 by amineau           #+#    #+#             */
/*   Updated: 2018/08/17 07:17:59 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

char*	ft_getcwd(void)
{
	size_t	len;
	size_t	buff_size;
	char*	cwd;
	char*	str;

	buff_size = 1024;
	len = buff_size;
	str = (char*)malloc(sizeof(char) * len);
	while ((cwd = getcwd(str, len)) == NULL && errno == ERANGE)
	{
		len += buff_size;
		// TODO : ft_realloc
		str = realloc(str, len);
	}
	return (cwd);
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