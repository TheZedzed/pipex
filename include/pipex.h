/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azeraoul <azeraoul@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 03:21:26 by azeraoul          #+#    #+#             */
/*   Updated: 2021/08/29 03:21:27 by azeraoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <stdio.h>
# include <sys/wait.h>
# include "libft.h"

typedef struct s_pipex
{
	char	**paths;
	char	**cmd;
	char	**env;
	char	**av;
	int		ac;
}t_pipex;

enum {
	PATH = -3,
	MALLOC = -1,
	SUCCESS = 0,
	REDIR = 1,
	FORK = 4,
	PIPE = 3,
	ARGS = 128,
	FOUND = 127,
};

void	free_split(char **el);
int		err_msg(t_pipex *el, int err);
int		init(t_pipex *el, char **env, char **av, int ac);
#endif
