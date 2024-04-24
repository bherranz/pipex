/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bherranz <bherranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 16:59:59 by bherranz          #+#    #+#             */
/*   Updated: 2024/04/24 12:32:03 by bherranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	redir_input(char *file)
{
	int	in;

	in = open(file, O_RDONLY);
	if (in < 0)
	{
		perror("Error while opening the file");
		return (1);
	}
	dup2(in, STDIN_FILENO);
	close(in);
	return (0);
}

int	redir_out(char *file)
{
	int	out;

	out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out < 0)
	{
		perror("Error while opening the file");
		return (1);
	}
	dup2(out, STDOUT_FILENO);
	close(out);
	return (0);
}

void	execute_cmd1(char **cmd, int *pipe1, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("Error forking");
		exit(1);
	}
	else if (pid == 0) //child process
	{
		dup2(pipe1[1], STDOUT_FILENO);
		close(pipe1[1]);
		if (execve(cmd[1], cmd, envp) == -1)
		{
			perror("Execution failed");
			exit(1);
		}
	}
	else //parent
	{
		close(pipe1[1]);
	}
}

int	main(int argc, char **argv, char **envp)
{
	//create the pipe
	int pipe1[2];

	pipe(pipe1);
	if (pipe1 < 0)
	{
		perror("Error creating pipe");
		exit (1);
	}
	if (argc != 5)
		exit(1);
	if (redir_input(argv[1]) == 1)
		exit (1);
	//execute command1, now STDIN is archivo 1 fd
	execute_cmd1(argv, pipe1, envp);
	// if (redir_output(argv[4]) == 1)
	// 	return (1);
	//execute command2, now STDOUT is archivo 2 fd
	//execute_cmd2(argv[3]);
	close(pipe1[1]);
	wait(NULL);
	return (0);
}

//Steps
/*
	create de pipe between first and second command
	redirect stdin to file2
	execute first command
	execute second command with the output of the first one
	redirect the output to file2
*/
