/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 05:56:41 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 02:42:16 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

void init_root_static(void)
{
	(void)get_root();
}

char *get_root(void)
{
	static char *root;

	if (!root)
		root = ft_getcwd();
	return (root);
}

char *get_wdir(void)
{
	char  *wdir;
	char  *cwd;
	char  *root;
	size_t root_len;
	size_t cwd_len;

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

int ftp_change_wdir(const char *dir)
{
	char  absolute_path[PATH_MAX];
	char *root_path;

	root_path = get_root();
	if (dir[0] == '/')
		dir = ft_strjoin(root_path, dir);
	realpath(dir, absolute_path);
	if (ft_strnstr(absolute_path, root_path, 1))
		return chdir(dir);
	return -1;
}