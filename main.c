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

void	process(t_pipe my_pipe)
{
	if (pipe(my_pipe.tube))
	{
		perror("Error creating the pipe");
		exit(1);
	}
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		perror("Error forking");
		exit (1);
	}
	else if (pid == 0)
	{
		close(STDOUT_FILENO);
		dup(my_pipe.tube[1]);
		close(my_pipe.tube[1]);
		redir_input(my_pipe.argv[1]);
		my_pipe.path = div_paths(get_path(my_pipe.envp));
		// encontrar el path del comando, gestionar si  no se encuentra el comando
		execve(*my_pipe.path, &my_pipe.argv[2], my_pipe.envp);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipe	my_pipe;

	if (argc != 5)
	{
		perror("Incorrect format");
		exit (1);
	}
	my_pipe.envp = envp;
	my_pipe.argv = argv;
	/*my_pipe.path = find_my_path(argv[1]);
	process(my_pipe);*/
	// para probar si estoy encontrando bien los paths
	char *path = get_path(envp);
	printf("%s\n", path);
	char **rutes = div_paths(path);
	while (*rutes)
	{
		printf("%s\n", *rutes);
		rutes++;
	}
	my_pipe.path = div_paths(get_path(my_pipe.envp));
	execve(*my_pipe.path, &my_pipe.argv[2], my_pipe.envp);
	exit(1);
}

/*
Steps:
	Encontrar path: buscar path en envp, despues buscar en esas carpetas
	y si no encuentras el comando, lo mandas sin path(el comando tal cual)
*/
