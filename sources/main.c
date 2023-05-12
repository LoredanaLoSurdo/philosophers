/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llo-surd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 09:35:15 by llo-surd          #+#    #+#             */
/*   Updated: 2023/03/15 15:53:35 by llo-surd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	start_simulation(t_actions *actions)
{
	unsigned int	i;

	actions->start_time = get_time_in_ms() + (actions->philo_count * 2 * 10);
	i = 0;
	while (i < actions->philo_count)
	{
		if (pthread_create(&actions->philos[i]->thread, NULL,
				&philosopher, actions->philos[i]) != 0)
			return (exit_code(STR_ERR_THREAD, NULL, actions));
		i++;
	}
	if (actions->philo_count > 1)
	{
		if (pthread_create(&actions->starvation, NULL,
				&starvation, actions) != 0)
			return (exit_code(STR_ERR_THREAD, NULL, actions));
	}
	return (true);
}

static void	stop_simulation(t_actions	*actions)
{
	unsigned int	i;

	i = 0;
	while (i < actions->philo_count)
	{
		pthread_join(actions->philos[i]->thread, NULL);
		i++;
	}
	if (actions->philo_count > 1)
		pthread_join(actions->starvation, NULL);
	if (DEBUG_FORMATTING == true && actions->times_to_eat != -1)
		write_outcome(actions);
	destroy_mutexes(actions);
	free_actions(actions);
}

int	main(int ac, char **av)
{
	t_actions	*actions;

	actions = NULL;
	if (ac - 1 < 4 || ac - 1 > 5)
		return (msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!input_check(ac, av))
		return (EXIT_FAILURE);
	actions = init_table(ac, av, 1);
	if (!actions)
		return (EXIT_FAILURE);
	if (!start_simulation(actions))
		return (EXIT_FAILURE);
	stop_simulation(actions);
	return (EXIT_SUCCESS);
}
