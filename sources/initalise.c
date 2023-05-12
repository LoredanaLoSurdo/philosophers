/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initalise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llo-surd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 10:57:56 by llo-surd          #+#    #+#             */
/*   Updated: 2023/03/15 15:58:13 by llo-surd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static pthread_mutex_t	*init_forks(t_actions *actions)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * actions->philo_count);
	if (!forks)
		return (init_error(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < actions->philo_count)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (init_error(STR_ERR_MUTEX, NULL, 0));
		i++;
	}
	return (forks);
}

static void	assign_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->actions->philo_count;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->actions->philo_count;
		philo->fork[1] = philo->id;
	}
}

static t_philo	**init_philosophers(t_actions *actions)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * actions->philo_count);
	if (!philos)
		return (init_error(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < actions->philo_count)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (init_error(STR_ERR_MALLOC, NULL, 0));
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
			return (init_error(STR_ERR_MUTEX, NULL, 0));
		philos[i]->actions = actions;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

static bool	init_mutexes(t_actions *actions)
{
	actions->fork_locks = init_forks(actions);
	if (!actions->fork_locks)
		return (false);
	if (pthread_mutex_init(&actions->sim_stop_lock, 0) != 0)
		return (exit_code(STR_ERR_MUTEX, NULL, actions));
	if (pthread_mutex_init(&actions->write_lock, 0) != 0)
		return (exit_code(STR_ERR_MUTEX, NULL, actions));
	return (true);
}

t_actions	*init_table(int ac, char **av, int i)
{
	t_actions	*actions;

	actions = malloc(sizeof(t_actions) * 1);
	if (!actions)
		return (init_error(STR_ERR_MALLOC, NULL, 0));
	actions->philo_count = unsigned_atoi(av[i++]);
	actions->time_to_die = unsigned_atoi(av[i++]);
	actions->time_to_eat = unsigned_atoi(av[i++]);
	actions->time_to_sleep = unsigned_atoi(av[i++]);
	actions->times_to_eat = -1;
	if (ac - 1 == 5)
		actions->times_to_eat = unsigned_atoi(av[i]);
	actions->philos = init_philosophers(actions);
	if (!actions->philos)
		return (NULL);
	if (!init_mutexes(actions))
		return (NULL);
	actions->sim_stop = false;
	return (actions);
}
