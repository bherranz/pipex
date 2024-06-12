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

void	ft_pids(t_pipex *pipex, int argc, pid_t **pids)
{
	*pids = ft_calloc((argc - 2 - pipex->here_doc), sizeof(pid_t));
	if (pipex->here_doc == 1)
		*pids[0] = process_here(pipex);
	else
		*pids[0] = process_in(pipex);
	pipex->pos += 2;
	while (pipex->argv[pipex->pos + 2])
	{
		pipex->prev[0] = pipex->current[0];
		pipex->prev[1] = pipex->current[1];
		if (pipe(pipex->current) < 0)
			print_error("Error creating the pipe", 1);
		(*pids)[pipex->pos - 2 - pipex->here_doc] = process_middle(pipex);
		close(pipex->prev[0]);
		close(pipex->prev[1]);
		pipex->pos++;
	}
	(*pids)[pipex->pos - 2 - pipex->here_doc] = process_out(pipex);
	close(pipex->current[0]);
	close(pipex->current[1]);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	pid_t	*pids;
	int		status;

	pipex.here_doc = 0;
	pipex.argv = argv;
	pipex.envp = envp;
	pipex.pos = 1;
	if (ft_strncmp(pipex.argv[pipex.pos], "here_doc",
			ft_strlen(pipex.argv[pipex.pos])) == 0)
		pipex.here_doc = 1;
	if ((argc < 5 && pipex.here_doc == 0) || (pipex.here_doc == 1 && argc < 6))
		print_error("Incorrect format", 1);
	if (pipe(pipex.current) < 0)
		print_error("Error creating the pipe", 1);
	pids = NULL;
	ft_pids(&pipex, argc, &pids);
	pipex.pos = 0;
	while (pids[pipex.pos])
	{
		waitpid(pids[pipex.pos], &status, 0);
		pipex.pos++;
	}
	free(pids);
	return (WEXITSTATUS(status));
}
