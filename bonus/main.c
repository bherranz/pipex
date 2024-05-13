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

void	process_in(char *file, char *cmd, char **envp, int *in_pipe)
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
		close(in_pipe[0]);
		dup2(fd, STDIN_FILENO);
		close(fd);
		dup2(in_pipe[1], STDOUT_FILENO);
		close(in_pipe[1]);
		execute(cmd, envp);
	}
	else
		waitpid(pid, &status, 0);
}

void	process_out(char *file, char *cmd, char **envp, int *in_pipe)
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
		close(in_pipe[1]);
		dup2(fd, STDOUT_FILENO); 
		close(fd);
		dup2(in_pipe[0], STDIN_FILENO);
		execute(cmd, envp);
	}
	else
	{
		close (in_pipe[0]);
		close (in_pipe[1]);
		waitpid(pid, &status, 0);
	}
}

void	process_middle(char *cmd, char **envp, int *in_pipe)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		print_error("Error forking");
	else if (pid == 0)
	{
		dup2(in_pipe[1], STDOUT_FILENO); 
		close(in_pipe[1]);
		dup2(in_pipe[0], STDIN_FILENO);
		execute(cmd, envp);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		my_pipe[2];
	int		i;

	if (argc < 5)
		print_error("Incorrect format");
	if (pipe(my_pipe) < 0)
		print_error("Error creating the pipe");
	printf("%s", *argv);
	process_in(argv[1], argv[2], envp, my_pipe);
	i = 3;
	while (argv[i + 2])
	{
		process_middle(argv[i], envp, my_pipe);
		i++;
	}
	process_out(argv[i + 1], argv[i], envp, my_pipe);
	close(my_pipe[0]);
	close(my_pipe[1]);
}

