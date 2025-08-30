/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybiloshy <ybiloshy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 21:15:06 by ybiloshy          #+#    #+#             */
/*   Updated: 2025/08/31 00:55:21 by ybiloshy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

int	ft_atoi(const char *n)
{
	int		sign;
	int		res;
	size_t	i;

	i = 0;
	sign = 1;
	res = 0;
	while (n[i] == 32 || (n[i] >= 9 && n[i] <= 13))
		i++;
	if (n[i] == '+' || n[i] == '-')
	{
		if (n[i] == '-')
			sign = -1;
		i++;
	}
	while (n[i] >= '0' && n[i] <= '9')
	{
		res = res * 10 + (n[i] - 48);
		i++;
	}
	if (res * sign < 0 || res * sign > 2147483647)
		return (-1);
	return (res * sign);
}

void	ft_free(t_philo **philos, t_data *data)
{
	int	i;

	pthread_mutex_destroy(&data->action);
	i = 0;
	while (i < data->amount)
	{
		pthread_mutex_destroy(&data->forks[i]);
		free(philos[i]);
		i++;
	}
	free(philos);
	free(data->forks);
}

void	ft_usleep(int time)
{
	struct timeval	start;
	struct timeval	now;
	long			diff;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&now, NULL);
		diff = (now.tv_sec * 1000 + now.tv_usec / 1000) - (start.tv_sec * 1000
				+ start.tv_usec / 1000);
		if (diff >= time)
			break ;
		usleep(100);
	}
}

int	get_time(struct timeval time)
{
	struct timeval	now;
	int				diff;

	gettimeofday(&now, NULL);
	diff = (now.tv_sec * 1000 + now.tv_usec / 1000) - (time.tv_sec * 1000
			+ time.tv_usec / 1000);
	return (diff);
}

void	t_run(t_philo **philos, t_fn_ptr philo_func, t_fn_ptr monitor_func,
		size_t n)
{
	pthread_t	*threads;
	pthread_t	monitor;
	size_t		i;

	threads = malloc(sizeof(pthread_t) * n);
	i = 0;
	while (i < n)
	{
		if (pthread_create(&threads[i], NULL, philo_func, philos[i]) != 0)
		{
			free(threads);
			return ;
		}
		i++;
	}
	pthread_create(&monitor, NULL, monitor_func, philos);
	i = 0;
	while (i < n)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	free(threads);
}
