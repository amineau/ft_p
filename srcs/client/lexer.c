/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 00:14:50 by amineau           #+#    #+#             */
/*   Updated: 2022/04/22 21:45:35 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"
#include "ftp_user_cmd_static.h"

t_server_verbs *ftp_cli_srv_lexer(char *str)
{
	int             i = 0;
	t_server_verbs *srv_verbs;
	char           *code;

	if (!str)
		return (NULL);
	while (str[i] && str[i] != ' ')
		i++;
	code = (char *)malloc(sizeof(char) * i + 1);
	ft_strncpy(code, str, i);
	if (!ftp_is_valid_response_code(code))
		return (NULL);
	if (!(srv_verbs = (t_server_verbs *)malloc(sizeof(t_server_verbs))))
		error_print_exit(EXIT_FAILURE_RETRY, "Malloc failed");
	srv_verbs->user_info = !str[i] ? ft_strdup("") : ft_strdup(&(str[i + 1]));
	srv_verbs->sr_code = ftp_get_ftp_code_enum(code);
	srv_verbs->sr_state = ftp_get_state_code(code);
	free(code);
	return srv_verbs;
}

int ftp_cli_user_lexer(const char *buff, t_client_verbs *cv)
{
	char **split;
	int    code_command;

	split = ft_strsplit(buff, ' ');
	if (!split[0])
		return (-1);
	else if ((code_command = ft_arraystr(g_user_cmd_str, split[0])) == -1)
	{
		printf("Unkwown command : [%s]\nType help for more information\n", split[0]);
		return (-1);
	}
	cv->cv_verb = split[0];
	cv->cv_arg = split[1];
	cv->cv_code = code_command;
	return (0);
}