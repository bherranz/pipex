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

void	process_in(char *file, char *cmd, char **envp, int *out_pipe)
{
	pid_t	pid;
	int		fd;
	int		status;

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
	else
		waitpid(pid, &status, 0);
}

void	process_out(char *file, char *cmd, char **envp, int *out_pipe)
{
	pid_t	pid;
	int		fd;
	int		status;

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
		waitpid(pid, &status, 0);
	}
}

void	process_middle(char *cmd, char **envp, int *in_pipe, int *out_pipe)
{
	pid_t	pid;
	int		status;
path
		execute(cmd, envp);
	}
	else
	{
		close (in_pipe[0]);
		close (in_pipe[1]);
		waitpid(pid, &status, 0);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int	in_pipe[2];
	int	out_pipe[2];
	int	i;

	if (argc < 5)
		print_error("Incorrect format");
	if (pipe(out_pipe) < 0)
		print_error("Error creating the pipe");
	process_in(argv[1], argv[2], envp, out_pipe);
	i = 3;
	while (argv[i + 2])
	{
		in_pipe[0] = out_pipe[0];
		in_pipe[1] = out_pipe[1];
		if (pipe(out_pipe) < 0)
			print_error("Error creating the pipe");
		process_middle(argv[i], envp, in_pipe, out_pipe);
		close(in_pipe[0]);
		close(in_pipe[1]);
		i++;
	}
	process_out(argv[i + 1], argv[i], envp, out_pipe);
	close(out_pipe[0]);
	close(out_pipe[1]);

	//return (WEXITSTATUS(status));
}
