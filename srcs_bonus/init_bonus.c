/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azeraoul <azeraoul@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 03:22:39 by azeraoul          #+#    #+#             */
/*   Updated: 2021/08/29 03:22:43 by azeraoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	split_len(char **el)
{
	int	len;

	len = 0;
	while (el[len])
		++len;
	return (len);
}

static char	*get_path(char **env)
{
	char	*res;
	int		i;

	i = 0;
	res = NULL;
	while (env[i])
	{
		if (!ft_strncmp("PATH=", env[i], 5))
		{
			res = env[i] + 5;
			break ;
		}
		++i;
	}
	return (res);
}

static char	**set_paths(char *line)
{
	char	**paths;
	char	**tmp;
	int		i;

	i = 0;
	paths = NULL;
	tmp = ft_split(line, ':');
	if (tmp)
	{
		paths = ft_calloc(split_len(tmp) + 1, sizeof(char *));
		if (paths)
		{
			while (tmp[i])
			{
				paths[i] = ft_strjoin(tmp[i], "/");
				++i;
			}
			free_split(tmp);
		}
	}
	return (paths);
}

int	init(t_pipex *el, char **env, char **av, int ac)
{
	char	*line;

	ft_memset(el, 0, sizeof(t_pipex));
	line = get_path(env);
	if (line)
	{
		el->paths = set_paths(line);
		if (el->paths)
		{
			if (ac > 5)
			{
				el->ac = ac;
				el->av = av;
				el->env = env;
				return (SUCCESS);
			}
			return (ARGS);
		}
		return (MALLOC);
	}
	return (PATH);
}
