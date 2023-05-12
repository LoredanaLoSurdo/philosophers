/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_status.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llo-surd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 14:28:27 by llo-surd          #+#    #+#             */
/*   Updated: 2023/03/15 16:08:23 by llo-surd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	sim_stop(t_actions *actions, bool status)
{
	pthread_mutex_lock(&actions->sim_stop_lock);
		actions->sim_stop = status;
	pthread_mutex_unlock(&actions->sim_stop_lock);
}

bool	has_simulation_stopped(t_actions *actions)
{
	bool	r;

	r = false;
	pthread_mutex_lock(&actions->sim_stop_lock);
	if (actions->sim_stop == true)
		r = true;
	pthread_mutex_unlock(&actions->sim_stop_lock);
	return (r);
}

static bool	kill_philo(t_philo *philo)
{
	time_t	time;

	time = get_time_in_ms();
	if ((time - philo->last_meal) >= philo->actions->time_to_die)
	{
		sim_stop(philo->actions, true);
		print_status(philo, true, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	return (false);
}

static bool	end_condition(t_actions *actions)
{
	unsigned int	i;
	bool			all_fed;

	all_fed = true;
	i = 0;
	while (i < actions->philo_count)
	{
		pthread_mutex_lock(&actions->philos[i]->meal_time_lock);
		if (kill_philo(actions->philos[i]))
			return (true);
		if (actions->times_to_eat != -1)
			if (actions->philos[i]->times_ate
				< (unsigned int)actions->times_to_eat)
				all_fed = false;
		pthread_mutex_unlock(&actions->philos[i]->meal_time_lock);
		i++;
	}
	if (actions->times_to_eat != -1 && all_fed == true)
	{
		sim_stop(actions, true);
		return (true);
	}
	return (false);
}

void	*starvation(void *data)
{
	t_actions			*actions;

	actions = (t_actions *)data;
	if (actions->times_to_eat == 0)
		return (NULL);
	sim_stop(actions, false);
	sim_start_delay(actions->start_time);
	while (true)
	{
		if (end_condition(actions) == true)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
