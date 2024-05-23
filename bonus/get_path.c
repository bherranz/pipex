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

	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			rutes = ft_split(*envp + 5, ':');
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

void	execute(char *argv, char **envp)
{
	char	**cmd;
	char	**rutes;
	char	*path;

	cmd = ft_split(argv, ' ');
	rutes = div_paths(envp);
	while (*rutes)
	{
		path = ft_strjoin(*rutes, cmd[0]);
		if (access(path, X_OK) == 0)
		{
			execve(path, cmd, envp);
			print_error("Fail in execution");
		}
		rutes++;
	}
	execve(argv, cmd, envp);
	print_error("Command not found");
}
