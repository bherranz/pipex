/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bherranz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 11:03:23 by bherranz          #+#    #+#             */
/*   Updated: 2024/06/12 11:03:26 by bherranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

pid_t	process_in(t_pipex *pipex)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid < 0)
		print_error("fork", 1);
	else if (pid == 0)
	{
		if (pipex->here_doc == 0)
			fd = open(pipex->argv[pipex->pos], O_RDONLY);
		else
			fd = pipex->here_fd;
		if (fd < 0)
			print_error(pipex->argv[pipex->pos], 1);
		close(pipex->current[0]);
		dup2(fd, STDIN_FILENO);
		close(fd);
		dup2(pipex->current[1], STDOUT_FILENO);
		close(pipex->current[1]);
		execute(pipex->argv[(pipex->pos) + 1 + (pipex->here_doc)], pipex->envp);
	}
	return (pid);
}

int	get_fd(t_pipex *pipex)
{
	int	fd;

	if (pipex->here_doc == 0)
		fd = open(pipex->argv[(pipex->pos) + 1],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(pipex->argv[(pipex->pos) + 1],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		print_error(pipex->argv[(pipex->pos) + 1], 1);
	return (fd);
}

pid_t	process_out(t_pipex *pipex)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid < 0)
		print_error("fork", 1);
	else if (pid == 0)
	{
		fd = get_fd(pipex);
		close(pipex->current[1]);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		dup2(pipex->current[0], STDIN_FILENO);
		execute(pipex->argv[pipex->pos], pipex->envp);
	}
	else
	{
		close (pipex->current[0]);
		close (pipex->current[1]);
	}
	return (pid);
}

pid_t	process_middle(t_pipex *pipex)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		print_error("fork", 1);
	else if (pid == 0)
	{
		dup2(pipex->prev[0], STDIN_FILENO);
		close(pipex->prev[0]);
		dup2(pipex->current[1], STDOUT_FILENO);
		close(pipex->current[0]);
		close(pipex->current[1]);
		close(pipex->prev[1]);
		execute(pipex->argv[pipex->pos], pipex->envp);
	}
	else
	{
		close (pipex->prev[0]);
		close (pipex->prev[1]);
	}
	return (pid);
}
