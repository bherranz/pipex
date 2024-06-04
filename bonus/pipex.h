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

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../libft/libft.h"

typedef struct s_pipex
{
	char	**envp;
	int		prev[2];
	int		current[2];
	char	**argv;
	int		pos;
}	t_pipex;

char	**div_paths(char **evnp);
void	execute(char *argv, char **envp);
void	print_error(char *msg, int err);
pid_t	process_here(t_pipex *pipex);

//gnl
char	*read_buffer(int fd, char *buffer);
char	*new_line(char *buffer);
char	*to_be_continued(char *buffer, char *line);
char	*get_next_line(int fd);
size_t	ft_strleng(const char *s);
void	*ft_callocg(size_t count, size_t size);
char	*ft_strchrg(const char *s, int c);
char	*ft_strjoing(char *s1, char *s2);
char	*ft_substrg(char *s, unsigned int start, size_t sub_len);

#endif
