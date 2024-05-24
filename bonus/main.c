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

void	print_error(char *msg)
{
	perror(msg);
	exit (1);
}

pid_t	process_in(char *file, char *cmd, char **envp, int *out_pipe)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid < 0)
		print_error("Error forking");
	else if (pid == 0)
	{
		fd = open(file, O_RDONLY);
		if (fd < 0)
			print_error("Error while opening the file");
		close(out_pipe[0]);
		dup2(fd, STDIN_FILENO);
		close(fd);
		dup2(out_pipe[1], STDOUT_FILENO);
		close(out_pipe[1]);
		execute(cmd, envp);
	}
	return (pid);
}

pid_t	process_out(char *file, char *cmd, char **envp, int *out_pipe)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid < 0)
		print_error("Error forking");
	else if (pid == 0)
	{
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			print_error("Error while opening the file");
		close(out_pipe[1]);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		dup2(out_pipe[0], STDIN_FILENO);
		execute(cmd, envp);
	}
	else
	{
		close (out_pipe[0]);
		close (out_pipe[1]);
	}
	return (pid);
}

pid_t	process_middle(char *cmd, char **envp, int *in_pipe, int *out_pipe)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		print_error("Error forking");
	else if (pid == 0)
	{
		dup2(in_pipe[0], STDIN_FILENO);
		close(in_pipe[0]);
		dup2(out_pipe[1], STDOUT_FILENO);
		close(out_pipe[0]);
		close(out_pipe[1]);
		close(in_pipe[1]);
		execute(cmd, envp);
	}
	else
	{
		close (in_pipe[0]);
		close (in_pipe[1]);
	}
	return (pid);
}

int	main(int argc, char **argv, char **envp)
{
	int		in_pipe[2];
	int		out_pipe[2];
	int		i;
	pid_t	*pids;
	int		status;

	if (argc < 5)
		print_error("Incorrect format");
	if (pipe(out_pipe) < 0)
		print_error("Error creating the pipe");
	pids = malloc(sizeof(pid_t)*(argc - 3));
	pids[argc - 3] = '\0';
	pids[0] = process_in(argv[1], argv[2], envp, out_pipe);
	i = 3;
	while (argv[i + 2])
	{
		in_pipe[0] = out_pipe[0];
		in_pipe[1] = out_pipe[1];
		if (pipe(out_pipe) < 0)
			print_error("Error creating the pipe");
		pids[i - 2] = process_middle(argv[i], envp, in_pipe, out_pipe);
		close(in_pipe[0]);
		close(in_pipe[1]);
		i++;
	}
	pids[i - 2] = process_out(argv[i + 1], argv[i], envp, out_pipe);
	close(out_pipe[0]);
	close(out_pipe[1]);
	while (*pids)
	{
		waitpid(*pids, &status, 0);
		pids++;
	}
	return (WEXITSTATUS(status));
}
