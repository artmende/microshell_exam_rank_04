/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artmende <artmende@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 14:44:45 by artmende          #+#    #+#             */
/*   Updated: 2022/01/03 17:58:37 by artmende         ###   ########.fr       */
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

	pid = fork();
	if (pid == -1)
		call_exit();
	if (pid == 0)
	{
		execve(arg_array[0], arg_array, env);
		write(STDERR_FILENO, "error: cannot execute ", 22);
		write(STDERR_FILENO, arg_array[0], ft_strlen(arg_array[0]));
		write(STDERR_FILENO, "\n", 1);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, 0, 0);
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
