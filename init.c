/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybiloshy <ybiloshy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 21:14:40 by ybiloshy          #+#    #+#             */
/*   Updated: 2025/08/30 23:21:06 by ybiloshy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

t_philo	**init_philos(int ac, char **av, t_data *data)
{
	int		amount;
	t_philo	**philos;
	int		i;

	amount = ft_atoi(av[1]);
	philos = malloc(sizeof(t_philo *) * amount);
	i = 0;
	while (i < amount)
	{
		philos[i] = malloc(sizeof(t_philo));
		philos[i]->id = i;
		philos[i]->time_to_die = ft_atoi(av[2]);
		philos[i]->time_to_eat = ft_atoi(av[3]);
		philos[i]->time_to_sleep = ft_atoi(av[4]);
		philos[i]->has_eaten = 0;
		if (ac == 6)
			philos[i]->must_eat = ft_atoi(av[5]);
		else
			philos[i]->must_eat = 0;
		philos[i]->last_time_philo_ate = 0;
		gettimeofday(&philos[i]->time, NULL);
		philos[i]->data = data;
		i++;
	}
	return (philos);
}

t_data	init_data(int amount)
{
	t_data	data;
	int		i;

	data.all_alive = 1;
	data.amount = amount;
	pthread_mutex_init(&data.action, NULL);
	data.forks = malloc(sizeof(pthread_mutex_t) * amount);
	i = 0;
	while (i < amount)
	{
		pthread_mutex_init(&data.forks[i], NULL);
		i++;
	}
	return (data);
}
