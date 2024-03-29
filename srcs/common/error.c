/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amineau <amineau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 21:33:50 by amineau           #+#    #+#             */
/*   Updated: 2022/04/23 20:07:14 by amineau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_p.h"

void error_print_exit(int status, const char *err)
{
	if (err)
		ft_printf("%s\n", err);
	exit(status);
}