/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artmende <artmende@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 14:44:50 by artmende          #+#    #+#             */
/*   Updated: 2022/01/05 14:59:37 by artmende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MICROSHELL_H
# define MICROSHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <signal.h>
# include <string.h>

#include <stdio.h>

/* microshell.c */
char	**get_next_pipe_args(int *index_args, char **semicolon_args);
void	execute_cmd(char **semicolon_args, char **env);
void	execute_cd(char **semicolon_args);
char	**get_semicolon_args(char **argv, int *argv_index);


/* microshell_utils.c */
void	call_exit(void);
int		ft_strlen(char *str);
char	**add_str_to_array(char **old, char *str);

#endif
