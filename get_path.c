/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bherranz <bherranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 12:26:59 by bherranz          #+#    #+#             */
/*   Updated: 2024/04/27 14:06:46 by bherranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**div_paths(char **envp)
{
	char	**rutes;
	int		i;

	rutes = NULL;
	while (envp && *envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
		{
			rutes = ft_split(*envp + 5, ':');
			break ;
		}
		envp++;
	}
	if (!rutes)
		return (NULL);
	i = 0;
	while (rutes[i])
	{
		rutes[i] = ft_strjoin(rutes[i], "/");
		i++;
	}
	return (rutes);
}

void	access_path(char **rutes, char **cmd, char **envp)
{
	char	*path;

	path = NULL;
	while (rutes && *rutes)
	{
		path = ft_strjoin(*rutes, cmd[0]);
		if (access(path, X_OK) == 0)
		{
			execve(path, cmd, envp);
			print_error("fail in execution", 127);
		}
		rutes++;
	}
	if (path)
		free(path);
}

void	execute(char *argv, char **envp)
{
	char	**cmd;

	cmd = ft_split(argv, ' ');
	if ((ft_strchr(argv, '/') != NULL))
	{
		execve(cmd[0], cmd, envp);
		print_error("fail in execution", 127);
	}
	access_path(div_paths(envp), cmd, envp);
	execve(cmd[0], cmd, envp);
	print_error("command not found", 127);
}
