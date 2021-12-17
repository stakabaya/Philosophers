/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakabay <stakabay@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 11:06:42 by stakabay          #+#    #+#             */
/*   Updated: 2021/12/18 01:32:20 by stakabay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

//Need to add check usleep time
void	smart_sleep(t_rules *rule, long long time)
{
	long long	i;

	i = timestamp();
	while (!(rule->died))
	{
		if (timestamp() - i >= time)
			break ;
		usleep(50);
	}
}

void	death_monitor(void *philos)
{
	t_rules	*rule;
	t_philo	*philo;
	int		rt;

	rt = 0;
	philo = philos;
	rule = philo->rules;
	while (!rule->died && !rule->all_ate)
	{
		rt |= pthread_mutex_lock(&rule->checking);
		if (timestamp() - philo->last_meal_time > rule->death && !rule->died)
		{
			rule->died = 1;
			rt |= pthread_mutex_lock(&rule->writing);
			printf("%lli %d %s\n", timestamp(), philo->id + 1, "died");
			rt |= pthread_mutex_unlock(&rule->writing);
		}
		rt |= pthread_mutex_unlock(&rule->checking);
	}
	if (rt)
		err_shutdown_threads(rule, "mutex error");
}

void	eatnum_monitor(void *rules)
{
	t_rules	*rule;
	int		i;
	int		rt;

	rt = 0;
	rule = rules;
	while (!rule->all_ate && !rule->died)
	{
		i = 0;
		while (i < rule->num_philos && rule->philos[i].num_meals >= \
				rule->eat_lim && rule->eat_lim != -1)
			i++;
		rt |= pthread_mutex_lock(&rule->checking);
		if (i == rule->num_philos)
		{
			rule->all_ate = 1;
			rt |= pthread_mutex_lock(&rule->writing);
			printf("all of philosophers are full\n");
			rt |= pthread_mutex_unlock(&rule->writing);
		}
		rt |= pthread_mutex_unlock(&rule->checking);
	}
	if (rt)
		err_shutdown_threads(rule, "mutex error");
}

void	eating(t_philo *philo, int id)
{
	int	rt;

	rt = 0;
	rt |= pthread_mutex_lock(&philo->rules->folks[id]);
	put_status(philo->rules, id, "taken a folk");
	if (id == philo->rules->num_philos - 1)
		rt |= pthread_mutex_lock(&philo->rules->folks[0]);
	else
		rt |= pthread_mutex_lock(&philo->rules->folks[id + 1]);
	put_status(philo->rules, id, "taken a folk");
	put_status(philo->rules, id, "is eating");
	(philo->num_meals)++;
	philo->last_meal_time = timestamp();
	smart_sleep(philo->rules, philo->rules->eat);
	rt |= pthread_mutex_unlock(&philo->rules->folks[id]);
	if (id == philo->rules->num_philos - 1)
		rt |= pthread_mutex_unlock(&philo->rules->folks[0]);
	else
		rt |= pthread_mutex_unlock(&philo->rules->folks[id + 1]);
	if (rt)
		err_shutdown_threads(philo->rules, "mutex error");
}

int	philo_action(void *philos)
{
	t_philo			*philo;
	int				id;

	philo = (t_philo *)philos;
	id = philo->id;
	philo->last_meal_time = timestamp();
	if (pthread_create(&philo->moni_thread, NULL, \
			(void *)death_monitor, (void *)philo))
		return (put_errmsg("create err"));
	if (id % 2)
		usleep(1000);
	while (!philo->rules->died && !philo->rules->all_ate)
	{
		eating(philo, id);
		if (philo->rules->all_ate || philo->rules->died)
			break ;
		put_status(philo->rules, id, "is sleeping");
		smart_sleep(philo->rules, philo->rules->sleep);
		put_status(philo->rules, id, "is thinking");
	}
	if (pthread_join(philo->moni_thread, NULL))
		return (put_errmsg("join err"));
	return (EXIT_SUCCESS);
}
