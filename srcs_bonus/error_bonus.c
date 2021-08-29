/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azeraoul <azeraoul@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 03:23:31 by azeraoul          #+#    #+#             */
/*   Updated: 2021/08/29 03:23:34 by azeraoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_split(char **el)
{
	int	i;

	if (el)
	{
		i = 0;
		while (el[i])
		{
			free(el[i]);
			++i;
		}
		free(el);
	}
}

int	err_msg(t_pipex *el, int err)
{
	if (err == MALLOC)
		write(2, "error -- malloc\n", 16);
	else if (err == ARGS)
		write(2, "error -- need args > 5\n", 23);
	else if (err == PATH)
		write(2, "error -- no path in env\n", 24);
	else if (err == REDIR)
		perror("error -- redirection");
	else if (err == FORK)
		perror("error -- fail fork");
	else if (err == PIPE)
		perror("error -- fail pipe");
	else if (err == FOUND)
	{
		write(2, el->cmd[0], ft_strlen(el->cmd[0]));
		write(2, ": command not found\n", 20);
	}
	free_split(el->paths);
	free_split(el->cmd);
	exit(err);
}
