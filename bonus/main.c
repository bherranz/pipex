/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bherranz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:37:11 by bherranz          #+#    #+#             */
/*   Updated: 2024/05/13 15:37:13 by bherranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	print_error(char *msg, int err)
{
	perror(msg);
	exit (err);
}

pid_t	process_in(t_pipex *pipex)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid < 0)
		print_error("Error forking", 1);
	else if (pid == 0)
	{
		fd = open(pipex->argv[pipex->pos], O_RDONLY);
		if (fd < 0)
			print_error("Error while opening the file", 1);
		close(pipex->current[0]);
		dup2(fd, STDIN_FILENO);
		close(fd);
		dup2(pipex->current[1], STDOUT_FILENO);
		close(pipex->current[1]);
		execute(pipex->argv[(pipex->pos) + 1], pipex->envp);
	}
	return (pid);
}

pid_t	process_out(t_pipex *pipex)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid < 0)
		print_error("Error forking", 1);
	else if (pid == 0)
	{
		fd = open(pipex->argv[(pipex->pos) + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			print_error("Error while opening the file", 1);
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
		print_error("Error forking", 1);
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

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	pid_t	*pids;
	int		status;

	if (argc < 5)
		print_error("Incorrect format", 1);
	if (pipe(pipex.current) < 0)
		print_error("Error creating the pipe", 1);
	pipex.argv = argv;
	pipex.envp = envp;
	pipex.pos = 1;
	if (ft_strncmp(pipex.argv[pipex.pos], "here_doc", ft_strlen(pipex.argv[pipex.pos])) == 0)
	{
		pids = ft_calloc((argc - 3), sizeof(pid_t));
		pids[0] = process_here(&pipex);	
		pipex.pos++;
	}
	else
	{
		pids = ft_calloc((argc - 2), sizeof(pid_t));
		pids[0] = process_in(&pipex);
	}
	pipex.pos += 2;
	while (argv[pipex.pos + 2])
	{
		pipex.prev[0] = pipex.current[0];
		pipex.prev[1] = pipex.current[1];
		if (pipe(pipex.current) < 0)
			print_error("Error creating the pipe", 1);
		pids[pipex.pos - 2] = process_middle(&pipex);
		close(pipex.prev[0]);
		close(pipex.prev[1]);
		pipex.pos++;
	}
	pids[pipex.pos - 4] = process_out(&pipex);
	close(pipex.current[0]);
	close(pipex.current[1]);
	pipex.pos = 0;
	while (pids[pipex.pos])
	{
		waitpid(pids[pipex.pos], &status, 0);
		pipex.pos++;
	}
	free(pids);
	return (WEXITSTATUS(status));
}
