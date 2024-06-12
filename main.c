/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bherranz <bherranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 16:59:59 by bherranz          #+#    #+#             */
/*   Updated: 2024/04/27 14:02:29 by bherranz         ###   ########.fr       */
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
		fd = open(pipex->argv[1], O_RDONLY);
		if (fd < 0)
			print_error("Error while opening the file", 1);
		close(pipex->current[0]);
		dup2(fd, STDIN_FILENO);
		close(fd);
		dup2(pipex->current[1], STDOUT_FILENO);
		close(pipex->current[1]);
		execute(pipex->argv[2], pipex->envp);
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
		fd = open(pipex->argv[4],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			print_error("Error while opening the file", 1);
		close(pipex->current[1]);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		dup2(pipex->current[0], STDIN_FILENO);
		execute(pipex->argv[3], pipex->envp);
	}
	else
	{
		close (pipex->current[0]);
		close (pipex->current[1]);
	}
	return (pid);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	pid_t	pids[2];
	int		status;

	pipex.argv = argv;
	pipex.envp = envp;
	if (argc != 5)
		print_error("Incorrect format", 1);
	if (pipe(pipex.current) < 0)
		print_error("Error creating the pipe", 1);
	pids[0] = process_in(&pipex);
	pids[1] = process_out(&pipex);
	close(pipex.current[0]);
	close(pipex.current[1]);
	waitpid(pids[0], &status, 0);
	waitpid(pids[1], &status, 0);
	return (WEXITSTATUS(status));
}
