/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakabay <stakabay@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 23:55:50 by stakabay          #+#    #+#             */
/*   Updated: 2021/12/11 11:15:27 by stakabay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

int	ft_ph_atoi(const char *str)
{
	long	num;
	int		sign;

	num = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\n' || \
			*str == '\v' || *str == '\f' || *str == '\r')
		str++;
	if (*str == '-')
		return (-1);
	else if (*str == '+')
		str++;
	while (*str)
	{
		if (*str < '0' || '9' < *str)
			return (-1);
		num = (num * 10) + (*str - '0');
		if (num < INT32_MIN || INT32_MAX < num)
			return (-1);
		str++;
	}
	return ((int)num);
}

void	init_args(t_rules *r, char **av)
{
	r->num_philos = ft_ph_atoi(av[1]);
	r->death = ft_ph_atoi(av[2]);
	r->eat = ft_ph_atoi(av[3]);
	r->sleep = ft_ph_atoi(av[4]);
	if (r->num_philos < 2 || r->death < 0 || r->eat < 0 || r->sleep < 0)
	{
		put_errmsg("invalid args");
		exit(EXIT_FAILURE);
	}
	if (av[5])
	{
		r->eat_lim = ft_ph_atoi(av[5]);
		if (r->eat_lim <= 0)
		{
			put_errmsg("invalid args");
			exit(EXIT_FAILURE);
		}
	}
	else
		r->eat_lim = -1;
}

void	init_philos(t_rules *rule)
{
	int	i;

	i = 0;
	while (i < rule->num_philos)
	{
		rule->philos[i].rules = rule;
		rule->philos[i].id = i;
		rule->philos[i].num_meals = 0;
		i++;
	}
}

void	init(t_rules *rule, char **av, int argc)
{
	if (argc != 5 && argc != 6)
		exit(put_errmsg("Invalid arguments num"));
	init_args(rule, av);
	rule->died = 0;
	rule->all_ate = 0;
	rule->folks = malloc(sizeof(pthread_mutex_t) * rule->num_philos);
	rule->philos = malloc(sizeof(t_philo) * rule->num_philos);
	rule->threads = malloc(sizeof(pthread_t) * rule->num_philos);
	if (!rule->folks || !rule->philos || !rule->threads)
		exit(put_errmsg("malloc error"));
	init_philos(rule);
}
