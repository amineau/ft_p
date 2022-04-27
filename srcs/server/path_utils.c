/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 05:56:41 by amineau           #+#    #+#             */
/*   Updated: 2022/04/27 21:26:22 by amineau          ###   ########.fr       */
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

t_bool start_with(const char *s1, const char *s2)
{
	return (ft_strnstr(s1, s2, 1) != NULL);
}

t_bool has_valid_relative_path(const char *path)
{
	char  absolute_path[PATH_MAX];
	char *root_path;

	root_path = get_root();
	if (path[0] == '/')
		path = ft_strjoin(root_path, path);
	if (realpath(path, absolute_path) != NULL)
		return (start_with(absolute_path, root_path));
	return false;
}

char *get_absolute_path(const char *path)
{
	char absolute_path[PATH_MAX];

	return realpath(path, absolute_path);
}

int ftp_change_wdir(const char *dir)
{
	if (has_valid_relative_path(dir))
		return chdir(dir);
	return -1;
}

int ftp_open_file(const char *pathname, int flags)
{
	if (has_valid_relative_path(pathname))

		return open(pathname, flags);
	return -1;
}