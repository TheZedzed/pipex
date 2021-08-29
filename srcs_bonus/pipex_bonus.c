#include "pipex_bonus.h"

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

static int	heredoc(char *delim)
{
	char	*line;
	int		res;
	int		fd;

	fd = open("/tmp/heredoc", O_WRONLY | O_CLOEXEC | O_CREAT, 0660);
	while (42)
	{
		res = get_next_line(0, &line);
		if (!res || !ft_strcmp(line, delim))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		if (res)
			write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	fd = open("/tmp/heredoc", O_CLOEXEC | O_RDONLY);
	unlink("/tmp/heredoc");
	return (fd);
}

static void	streams(t_pipex *el, int in, int out, int pos)
{
	if (pos == el->ac - 2)
	{
		if (!ft_strcmp(el->av[1], "here_doc"))
			out = open(el->av[pos + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
		else
			out = open(el->av[pos + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (out < 0)
			err_msg(el, REDIR);
	}
	else if (pos == 2)
	{
		in = open(el->av[pos - 1], O_RDONLY);
		if (in < 0)
			err_msg(el, REDIR);
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
	int		status;
	int		err;

	err = init(&el, env, av, ac);
	if (!err)
	{
		if (!ft_strcmp(av[1], "here_doc"))
			execute(&el, 3, heredoc(av[2]));
		else
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
