/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakabay <stakabay@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 11:13:23 by stakabay          #+#    #+#             */
/*   Updated: 2021/12/11 11:14:04 by stakabay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

long long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

//mutex_initは常に1を返し、失敗しない
void	game_start(t_rules *rule)
{
	int	i;
	int	rt;

	rt = 0;
	i = 0;
	while (i < rule->num_philos)
	{
		pthread_mutex_init(&rule->folks[i], NULL);
		rt |= pthread_create(&rule->threads[i], NULL, \
								(void *)philo_action, (void *)&rule->philos[i]);
		i++;
	}
	pthread_mutex_init(&rule->writing, NULL);
	pthread_mutex_init(&rule->checking, NULL);
	rt |= pthread_create(&rule->eat_moni_thread, NULL, \
						(void *)eatnum_monitor, (void *)rule);
	if (rt)
		ft_error_exit(rule, "threads create error");
}

void	game_end(t_rules *rule)
{
	int	i;
	int	rt;

	rt = 0;
	i = 0;
	while (i < rule->num_philos)
	{
		rt |= pthread_join(rule->threads[i], NULL);
		rt |= pthread_mutex_destroy(&rule->folks[i]);
		i++;
	}
	rt |= pthread_join(rule->eat_moni_thread, NULL);
	rt |= pthread_mutex_destroy(&rule->writing);
	rt |= pthread_mutex_destroy(&rule->checking);
	if (rt)
		ft_error_exit(rule, "thread join or mutex destroy error");
}

void	put_status(t_rules *rule, int id, char *str)
{
	long long	time;

	if (pthread_mutex_lock(&rule->writing))
		err_shutdown_threads(rule, "mutex lock error");
	time = timestamp();
	if (rule->died == 0 && rule->all_ate == 0)
	{
		printf("%lli %d %s\n", time, id, str);
	}
	if (pthread_mutex_unlock(&rule->writing))
		err_shutdown_threads(rule, "mutex error");
}

int	main(int argc, char **argv)
{
	t_rules			rule;

	init(&rule, argv, argc);
	game_start(&rule);
	game_end(&rule);
	ft_free(&rule);
	return (0);
}
