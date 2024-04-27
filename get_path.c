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

char	*get_path(char **envp)
{
	while (**envp)
	{
		if (strncmp(*envp, "PATH=", 5) == 0)
		{	
			*envp = *envp + 5;
			return (*envp);
		}
		envp++;
	}
	return (NULL);
}

char	**div_paths(char *path)
{
	char	**rutes;
	int		i;

	rutes = ft_split(path, ':');
	i = 0;
	while (rutes[i])
	{
		rutes[i] = ft_strjoin(rutes[i], "/");
		i++;
	}
	return (rutes);
}

