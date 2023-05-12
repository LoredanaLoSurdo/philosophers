/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_checks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llo-surd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 16:36:07 by llo-surd          #+#    #+#             */
/*   Updated: 2023/03/15 16:09:13 by llo-surd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	isdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	unsigned_atoi(char *str)
{
	unsigned long long int	res;
	int						i;

	i = 0;
	res = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	if (res > INT_MAX)
		return (-1);
	return ((int)res);
}

bool	input_check(int ac, char **av)
{
	int	i;
	int	n;

	i = 1;
	while (i < ac)
	{
		if (!isdigit(av[i]))
			return (msg(STR_ERR_INPUT_DIGIT, av[i], false));
		n = unsigned_atoi(av[i]);
		if (i == 1 && (n <= 0 || n > MAX_PHILOS))
			return (msg(STR_ERR_INPUT_POFLOW, STR_MAX_PHILOS, false));
		if (i != 1 && n == -1)
			return (msg(STR_ERR_INPUT_DIGIT, av[i], false));
		i++;
	}
	return (true);
}
