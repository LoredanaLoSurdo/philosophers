/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llo-surd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 11:28:28 by llo-surd          #+#    #+#             */
/*   Updated: 2023/03/15 15:53:35 by llo-surd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>
# define MAX_PHILOS	200
# define STR_MAX_PHILOS "200"
# ifndef DEBUG_FORMATTING
#  define DEBUG_FORMATTING 0
# endif
# define NC		"\e[0m"
# define RED	"\e[31m"
# define GREEN	"\e[32m"
# define PURPLE	"\e[35m"
# define CYAN	"\e[36m"
# define STR_PROG_NAME	"philo:"
# define STR_USAGE	"%s usage: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n"
# define STR_ERR_INPUT_DIGIT	"%s invalid input: %s: \
not a valid unsigned integer between 0 and 2147483647.\n"
# define STR_ERR_INPUT_POFLOW	"%s invalid input: \
there must be between 1 and %s philosophers.\n"
# define STR_ERR_THREAD	"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC	"%s error: Could not allocate memory.\n"
# define STR_ERR_MUTEX	"%s error: Could not create mutex.\n"

typedef struct s_philo	t_philo;

typedef struct s_actions
{
	time_t			start_time;
	unsigned int	philo_count;
	pthread_t		starvation;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				times_to_eat;
	bool			sim_stop;
	pthread_mutex_t	sim_stop_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			**philos;
}	t_actions;

typedef struct s_philo
{
	pthread_t			thread;
	unsigned int		id;
	unsigned int		times_ate;
	unsigned int		fork[2];
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_actions			*actions;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_status;

bool			input_check(int ac, char **av);
int				unsigned_atoi(char *str);

t_actions		*init_table(int ac, char **av, int i);

void			*philosopher(void *data);

time_t			get_time_in_ms(void);
void			philo_sleep(t_actions *actions, time_t sleep_time);
void			sim_start_delay(time_t start_time);
void			print_status(t_philo *philo, bool reaper, t_status status);
void			write_outcome(t_actions *actions);
void			*init_error(char *str, char *details, t_actions *actions);
int				msg(char *str, char *detail, int exit_no);
void			*starvation(void *data);
bool			has_simulation_stopped(t_actions *actions);
int				exit_code(char *str, char *details, t_actions *actions);
void			*free_actions(t_actions *actions);
void			destroy_mutexes(t_actions *actions);

#endif