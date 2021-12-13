/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakabay <stakabay@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 11:11:11 by stakabay          #+#    #+#             */
/*   Updated: 2021/12/11 17:05:19 by stakabay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <pthread.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;
	struct s_rules	*rules;
	int				num_meals;
	long long		last_meal_time;
	pthread_t		moni_thread;
}				t_philo;

typedef struct s_rules
{
	t_philo			*philos;
	pthread_mutex_t	*folks;
	pthread_mutex_t	writing;
	pthread_mutex_t	checking;
	pthread_t		*threads;
	pthread_t		eat_moni_thread;
	int				num_philos;
	int				death;
	int				sleep;
	int				eat;
	int				died;
	int				eat_lim;
	int				all_ate;

}				t_rules;

void		ft_free(t_rules *rule);
int			put_errmsg(char *str);
int			ft_error_exit(t_rules *rule, char *str);
void		err_shutdown_threads(t_rules *rule, char *str);
int			ft_ph_atoi(const char *str);
int			init_args(t_rules *r, char **av);
void		init_philos(t_rules *rule);
int			init(t_rules *rule, char **av, int argc);
int			philo_action(void *philos);
void		eating(t_philo *philo, int id);
void		eatnum_monitor(void *rules);
void		death_monitor(void *philos);
void		smart_sleep(t_rules *rule, long long time);
void		put_status(t_rules *rule, int id, char *str);
long long	timestamp(void);

#endif
