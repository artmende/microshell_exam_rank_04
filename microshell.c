/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artmende <artmende@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 14:44:45 by artmende          #+#    #+#             */
/*   Updated: 2022/01/05 15:03:56 by artmende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

char	**get_next_pipe_args(int *index_args, char **semicolon_args)
{
	char	**ret = NULL;

	if (semicolon_args[*index_args] == NULL)
		return (NULL);
	while (semicolon_args[*index_args] && strcmp(semicolon_args[*index_args], "|"))
	{
		ret = add_str_to_array(ret, semicolon_args[*index_args]);
		*index_args = *index_args + 1;
	}
	if (semicolon_args[*index_args]) // if we finished on a pipe
		*index_args = *index_args + 1;
	return (ret);
}

void	execute_cmd(char **semicolon_args, char **env)
{
	pid_t	pid;
	int		index_args = 0;
	char	**pipe_section;
	int		fd[2]; // fd[0] is read. fd[1] is write.
	int		oldfdin = 0;

	while ((pipe_section = get_next_pipe_args(&index_args, semicolon_args)))
	{
		if (semicolon_args[index_args]) // true for all commands except the last one
		{
			if (pipe(fd) == -1)
				call_exit();
		}
		pid = fork();
		if (pid == -1)
			call_exit();
		if (pid == 0)
		{
			if (semicolon_args[index_args])
			{
				if (-1 == dup2(fd[1], STDOUT_FILENO))
					call_exit();
			}
			if (oldfdin) // no need to modify the stdin for the first command
			{
				if (-1 == dup2(oldfdin, STDIN_FILENO))
					call_exit();
			}
			execve(pipe_section[0], pipe_section, env);
			write(STDERR_FILENO, "error: cannot execute ", 22);
			write(STDERR_FILENO, pipe_section[0], ft_strlen(pipe_section[0]));
			write(STDERR_FILENO, "\n", 1);
			exit(EXIT_FAILURE);
		}
		waitpid(pid, 0, 0);
		if (oldfdin != 0) // do not close stdin
			close(oldfdin); // closing the read end of the pipe from the previous cycle
		oldfdin = fd[0];
		close(fd[1]); // sending EOF to the next process
		free(pipe_section);
	}
}

void	execute_cd(char **semicolon_args)
{
	if (semicolon_args[1] == NULL)
	{
		return ;
	}
	else if (semicolon_args[2])
	{
		write(STDERR_FILENO, "error: cd: bad arguments\n", 25);
	}
	else
	{
		if (-1 == chdir(semicolon_args[1]))
		{
			write(STDERR_FILENO, "error: cd: cannot change directory to ", 38);
			write(STDERR_FILENO, semicolon_args[1], ft_strlen(semicolon_args[1]));
			write(STDERR_FILENO, "\n", 1);
		}
	}
}

char	**get_semicolon_args(char **argv, int *argv_index)
{
	char	**ret = NULL;

	while (argv[*argv_index] && strcmp(";", argv[*argv_index]))
	{
		ret = add_str_to_array(ret, argv[*argv_index]);
		*argv_index = *argv_index + 1;
	}
	if (argv[*argv_index])
		*argv_index = *argv_index + 1;
	return (ret);
}

int	main(int argc, char **argv, char **env)
{
	int		argv_index = 1; // 0 is microshell
	char	**semicolon_args;

	while (argv_index < argc) // argv[argc] is always NULL
	{
		semicolon_args = get_semicolon_args(argv, &argv_index);
		if (semicolon_args == NULL) // in cases like that --> ; ;
			continue ;
		if (0 == strcmp("cd", semicolon_args[0]))
		{
			execute_cd(semicolon_args);
			free(semicolon_args);
			continue ;
		}
		execute_cmd(semicolon_args, env);
		free(semicolon_args);
	}
	return (0);
}
