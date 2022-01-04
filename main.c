/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artmende <artmende@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 14:44:45 by artmende          #+#    #+#             */
/*   Updated: 2022/01/04 15:44:33 by artmende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void	call_exit(void)
{
	write(STDERR_FILENO, "error: fatal\n", 13);
	exit(EXIT_FAILURE);
}

int	ft_strlen(char *str)
{
	int	i = 0;

	while (str && str[i])
		i++;
	return (i);
}

char	**duplicate_array_from_index(char **old, int index_from, int index_to)
{
	char	**ret;
	int		i = 0;

	if (index_from < 0 || index_to < 0 || index_to < index_from)
		return (NULL);
	ret = malloc(sizeof(char *) * (2 + index_to - index_from));
	if (!ret)
		call_exit();
	while (index_from <= index_to)
	{
		ret[i] = old[index_from];
		i++;
		index_from++;
	}
	ret[i] = NULL;
	return (ret);
}

char	**get_next_pipe_args(int *index_to, char **arg_array)
{
	int		index_from = *index_to;
	char	**ret;

	if (arg_array[*index_to] == NULL)
		return (NULL);
	while (arg_array[*index_to] && strncmp(arg_array[*index_to], "|", ft_strlen(arg_array[*index_to])))
		*index_to = *index_to + 1;
	ret = duplicate_array_from_index(arg_array, index_from, *index_to - 1);
	if (arg_array[*index_to]) // if we finished on a pipe
		*index_to = *index_to + 1;
	return (ret);
}

char	**add_str_to_array(char **old, char *str)
{
	int		i = 0;
	char	**ret;

	while (old && old[i])
		i++;
	ret = malloc(sizeof(char *) * (i + 2));
	if (!ret)
		call_exit();
	ret[i + 1] = NULL;
	ret[i] = str;
	while (i)
	{
		i--;
		ret[i] = old[i];
	}
	free(old);
	return (ret);
}

char	**get_arg_array(char **argv, int *argv_index)
{
	char	**ret = NULL;

	while (argv[*argv_index] && strncmp(";", argv[*argv_index], ft_strlen(argv[*argv_index])))
	{
		ret = add_str_to_array(ret, argv[*argv_index]);
		*argv_index = *argv_index + 1;
	}
	if (argv[*argv_index])
		*argv_index = *argv_index + 1;
	return (ret);
}

void	execute_cmd(char **arg_array, char **env)
{
	pid_t	pid;
	int		index_to = 0;
	char	**pipe_section;
	int		fd[2];
	int		oldfdin = STDIN_FILENO;

	while ((pipe_section = get_next_pipe_args(&index_to, arg_array)))
	{
		if (arg_array[index_to])
		{
			if (pipe(fd) == -1)
				call_exit();
		}
		pid = fork();
		if (pid == -1)
			call_exit();
		if (pid == 0)
		{
			if (arg_array[index_to])
				dup2(fd[1], STDOUT_FILENO);
			if (oldfdin)
				dup2(oldfdin, STDIN_FILENO);
			execve(pipe_section[0], pipe_section, env);
			write(STDERR_FILENO, "error: cannot execute ", 22);
			write(STDERR_FILENO, pipe_section[0], ft_strlen(pipe_section[0]));
			write(STDERR_FILENO, "\n", 1);
			exit(EXIT_FAILURE);
		}
		waitpid(pid, 0, 0);
		oldfdin = fd[0];
		close(fd[1]); /////////////
		free(pipe_section);
	}
}

void	execute_cd(char **arg_array, char **env)
{
	
}

int	main(int argc, char **argv, char **env)
{
	int		argv_index = 1;
	char	**arg_array;

	while (argv_index < argc) // argv[argc] is always NULL
	{
		arg_array = get_arg_array(argv, &argv_index);
		if (arg_array == NULL)
			continue ;
		if (0 == strncmp("cd", arg_array[0], ft_strlen(arg_array[0])))
		{
			execute_cd(arg_array, env);
			free(arg_array);
			continue ;
		}
		execute_cmd(arg_array, env);
		free(arg_array);
	}



	return (0);
}
