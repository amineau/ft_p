/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 00:14:50 by amineau           #+#    #+#             */
/*   Updated: 2022/04/24 14:20:59 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"
#include "ftp_user_cmd_static.h"

t_srv_res *ftp_cli_srv_lexer(char *str)
{
	int        i = 0;
	t_srv_res *srv_verbs;
	char      *code;

	if (!str)
		return (NULL);
	while (str[i] && str[i] != ' ')
		i++;
	code = (char *)malloc(sizeof(char) * i + 1);
	ft_strncpy(code, str, i);
	if (!ftp_is_valid_response_code(code))
		return (NULL);
	if (!(srv_verbs = (t_srv_res *)malloc(sizeof(t_srv_res))))
		error_print_exit(EXIT_FAILURE_RETRY, "Malloc failed");
	srv_verbs->user_info = !str[i] ? ft_strdup("") : ft_strdup(&(str[i + 1]));
	srv_verbs->sr_code = ftp_get_code_enum(code);
	free(code);
	return srv_verbs;
}

t_user_verbs *ftp_cli_user_lexer(const char *str)
{
	char        **split;
	int           code_command;
	t_user_verbs *uv;

	split = ft_strsplit(str, ' ');
	if (!split[0])
		return (NULL);
	else if ((code_command = ft_arraystr(user_cmd_str, split[0])) == -1)
	{
		ft_printf("Unkwown command : [%s]\nType help for more information\n",
				  split[0]);
		return (NULL);
	}
	if (!(uv = (t_user_verbs *)malloc(sizeof(t_user_verbs))))
		error_print_exit(EXIT_FAILURE_RETRY, "Malloc failed");
	uv->uv_arg = split[1];
	uv->uv_code = (t_user_cmd)code_command;
	return (uv);
}