/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llo-surd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 14:45:42 by llo-surd          #+#    #+#             */
/*   Updated: 2023/03/15 16:00:21 by llo-surd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_loop(t_philo *philo)
{
	pthread_mutex_lock(&philo->actions->fork_locks[philo->fork[0]]);
	print_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->actions->fork_locks[philo->fork[1]]);
	print_status(philo, false, GOT_FORK_2);
	print_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);
	philo_sleep(philo->actions, philo->actions->time_to_eat);
	if (has_simulation_stopped(philo->actions) == false)
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate += 1;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
	print_status(philo, false, SLEEPING);
	pthread_mutex_unlock(&philo->actions->fork_locks[philo->fork[1]]);
	pthread_mutex_unlock(&philo->actions->fork_locks[philo->fork[0]]);
	philo_sleep(philo->actions, philo->actions->time_to_sleep);
}

static void	think_loop(t_philo *philo, bool silent)
{
	time_t	time_to_think;

	pthread_mutex_lock(&philo->meal_time_lock);
	time_to_think = (philo->actions->time_to_die
			- (get_time_in_ms() - philo->last_meal)
			- philo->actions->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == false)
		print_status(philo, false, THINKING);
	philo_sleep(philo->actions, time_to_think);
}

static void	*solo_philo_loop(t_philo *philo)
{
	pthread_mutex_lock(&philo->actions->fork_locks[philo->fork[0]]);
	print_status(philo, false, GOT_FORK_1);
	philo_sleep(philo->actions, philo->actions->time_to_die);
	print_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->actions->fork_locks[philo->fork[0]]);
	return (NULL);
}

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->actions->times_to_eat == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = philo->actions->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	sim_start_delay(philo->actions->start_time);
	if (philo->actions->time_to_die == 0)
		return (NULL);
	if (philo->actions->philo_count == 1)
		return (solo_philo_loop(philo));
	else if (philo->id % 2)
		think_loop(philo, true);
	while (has_simulation_stopped(philo->actions) == false)
	{
		philo_loop(philo);
		think_loop(philo, false);
	}
	return (NULL);
}
