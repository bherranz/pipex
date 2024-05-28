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

#include "../pipex"

void *here_doc(char *limit, int *my_pipe)
{
	char	*file;
	char	*line;

	while (1)
	{
		line = get_next_line(0);
		if (ft_strncmp(line, limit) == 0)
			break ;
		ft_putstr_fd(line, my_pipe[1]);
	}
}
