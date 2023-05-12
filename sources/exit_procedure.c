/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_procedure.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llo-surd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 10:56:09 by llo-surd          #+#    #+#             */
/*   Updated: 2023/03/15 15:57:10 by llo-surd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*free_actions(t_actions *actions)
{
	unsigned int	i;

	if (!actions)
		return (NULL);
	if (actions->fork_locks != NULL)
		free(actions->fork_locks);
	if (actions->philos != NULL)
	{
		i = 0;
		while (i < actions->philo_count)
		{
			if (actions->philos[i] != NULL)
				free(actions->philos[i]);
			i++;
		}
		free(actions->philos);
	}
	free(actions);
	return (NULL);
}

void	destroy_mutexes(t_actions *actions)
{
	unsigned int	i;

	i = 0;
	while (i < actions->philo_count)
	{
		pthread_mutex_destroy(&actions->fork_locks[i]);
		pthread_mutex_destroy(&actions->philos[i]->meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&actions->write_lock);
	pthread_mutex_destroy(&actions->sim_stop_lock);
}

int	msg(char *str, char *error, int exit_no)
{
	if (!error)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, error);
	return (exit_no);
}

int	exit_code(char *str, char *error, t_actions *actions)
{
	if (actions != NULL)
		free_actions(actions);
	return (msg(str, error, 0));
}

void	*init_error(char *str, char *error, t_actions *actions)
{
	if (actions != NULL)
		free_actions(actions);
	msg(str, error, EXIT_FAILURE);
	return (NULL);
}
