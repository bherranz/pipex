/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bherranz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:06:54 by bherranz          #+#    #+#             */
/*   Updated: 2024/05/28 12:06:56 by bherranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	here_doc(char *limit, int *my_pipe)
{
	char	*line;
	size_t	max_len;

	while (1)
	{
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strlen(line) > ft_strlen(limit))
			max_len = ft_strlen(line) - 1;
		else
			max_len = ft_strlen(limit);
		if (ft_strncmp(line, limit, max_len) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, my_pipe[1]);
		free(line);
	}
}

pid_t	process_here(t_pipex *pipex)
{
	pid_t	pid;
	int		here_pipe[2];

	pid = fork();
	if (pid < 0)
		print_error("Error forking", 1);
	else if (pid == 0)
	{
		if (pipe(here_pipe) < 0)
			print_error("Error creating the pipe", 1);
		here_doc(pipex->argv[(pipex->pos) + 1], here_pipe);
		dup2(here_pipe[0], STDIN_FILENO);
		close(here_pipe[0]);
		dup2(pipex->current[1], STDOUT_FILENO);
		close(pipex->current[0]);
		close(pipex->current[1]);
		close(here_pipe[1]);
		execute(pipex->argv[(pipex->pos) + 2], pipex->envp);
	}
	waitpid(pid, NULL, 0);
	return (pid);
}
