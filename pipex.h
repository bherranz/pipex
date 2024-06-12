/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bherranz <bherranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 17:00:38 by bherranz          #+#    #+#             */
/*   Updated: 2024/04/27 14:01:10 by bherranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "libft/libft.h"

typedef struct s_pipex
{
	char	**envp;
	int		current[2];
	char	**argv;
}	t_pipex;

char	**div_paths(char **evnp);
void	execute(char *argv, char **envp);
void	print_error(char *msg, int err);

#endif
