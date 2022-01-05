/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artmende <artmende@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 14:50:40 by artmende          #+#    #+#             */
/*   Updated: 2022/01/05 14:51:12 by artmende         ###   ########.fr       */
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
