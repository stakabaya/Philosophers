/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakabay <stakabay@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 11:14:29 by stakabay          #+#    #+#             */
/*   Updated: 2021/12/11 16:15:51 by stakabay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	ft_free(t_rules *rule)
{
	free(rule->philos);
	free(rule->threads);
	free(rule->folks);
}

int	put_errmsg(char *str)
{
	printf("Error : %s\n", str);
	return (EXIT_FAILURE);
}

int	ft_error_exit(t_rules *rule, char *str)
{
	put_errmsg(str);
	ft_free(rule);
	return (EXIT_FAILURE);
}

void	err_shutdown_threads(t_rules *rule, char *str)
{
	put_errmsg(str);
	rule->died = 1;
}
