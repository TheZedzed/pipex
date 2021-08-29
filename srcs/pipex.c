/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azeraoul <azeraoul@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/29 03:21:59 by azeraoul          #+#    #+#             */
/*   Updated: 2021/08/29 03:22:02 by azeraoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	command(t_pipex *el, char *av)
{
	char	*tmp;
	int		i;

	free_split(el->cmd);
	el->cmd = ft_split(av, ' ');
	if (!el->cmd)
		err_msg(el, MALLOC);
	i = 0;
	while (el->cmd[0] && el->paths[i])
	{
		tmp = ft_strjoin(el->paths[i], el->cmd[0]);
		if (!access(tmp, F_OK))
		{
			free(el->cmd[0]);
			el->cmd[0] = tmp;
			return (SUCCESS);
		}
		free(tmp);
		++i;
	}
	return (1);
}

static void	streams(t_pipex *el, int in, int out, int pos)
{
	if (pos == el->ac - 2)
	{
		out = open(el->av[pos + 1], O_RDWR | O_TRUNC | O_CREAT, 0660);
		if (out < 0)
			err_msg(el, REDIR);
	}
	else
	{
		if (pos == 2)
		{
			in = open(el->av[pos - 1], O_RDONLY);
			if (in < 0)
				err_msg(el, REDIR);
		}
	}
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
}

static void	execute(t_pipex *el, int pos, int in)
{
	int	pipefd[2];
	int	none;
	int	pid;

	if (pos == el->ac - 1 && !close(in))
		return ;
	none = command(el, el->av[pos]);
	if (pipe(pipefd))
		err_msg(el, PIPE);
	pid = fork();
	if (pid < 0)
		err_msg(el, FORK);
	else if (pid == 0)
	{
		close(pipefd[0]);
		streams(el, in, pipefd[1], pos);
		if (!none)
			execve(el->cmd[0], el->cmd, el->env);
		err_msg(el, FOUND);
	}
	if (in != STDIN_FILENO)
		close(in);
	close(pipefd[1]);
	return (execute(el, pos + 1, pipefd[0]));
}

int	main(int ac, char **av, char **env)
{
	t_pipex	el;
	int		err;
	int		status;

	err = init(&el, env, av, ac);
	if (!err)
	{
		execute(&el, 2, STDIN_FILENO);
		while (wait(&status) > 0)
		{
			if (WIFEXITED(status))
				err = WEXITSTATUS(status);
		}
	}
	else
		err_msg(&el, err);
	free_split(el.cmd);
	free_split(el.paths);
	return (err);
}
