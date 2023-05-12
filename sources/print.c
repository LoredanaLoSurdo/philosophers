/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llo-surd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 14:41:29 by llo-surd          #+#    #+#             */
/*   Updated: 2023/03/15 15:50:45 by llo-surd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_log(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_time_in_ms() - philo->actions->start_time,
		philo->id + 1, str);
}

void	print_status(t_philo *philo, bool death_log, t_status status)
{
	pthread_mutex_lock(&philo->actions->write_lock);
	if (has_simulation_stopped(philo->actions) == true && death_log == false)
	{
		pthread_mutex_unlock(&philo->actions->write_lock);
		return ;
	}
	if (status == DIED)
		print_log(philo, "died");
	else if (status == EATING)
		print_log(philo, "is eating");
	else if (status == SLEEPING)
		print_log(philo, "is sleeping");
	else if (status == THINKING)
		print_log(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		print_log(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->actions->write_lock);
}
