/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybiloshy <ybiloshy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 21:14:54 by ybiloshy          #+#    #+#             */
/*   Updated: 2025/08/31 00:08:24 by ybiloshy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

void	action(t_philo *philo, char *str, int time)
{
	pthread_mutex_lock(&philo->data->action);
	if (philo->data->all_alive)
		printf("%i %d %s\n", get_time(philo->time), philo->id, str);
	pthread_mutex_unlock(&philo->data->action);
	ft_usleep(time);
}

int	is_one_philo(t_philo *philo)
{
	if (philo->data->amount == 1)
	{
		ft_usleep(philo->time_to_die);
		printf("%i %d %s\n", get_time(philo->time), philo->id, "died");
		philo->data->all_alive = 0;
		pthread_mutex_unlock(&philo->data->forks[0]);
		return (1);
	}
	return (0);
}

int	take_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id;
	right = philo->id + 1;
	if (right == philo->data->amount)
		right = 0;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->forks[left]);
		action(philo, "has taken the left fork", 0);
		if(is_one_philo(philo))
			return(0);
		pthread_mutex_lock(&philo->data->forks[right]);
		action(philo, "has taken the right fork", 0);
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[right]);
		action(philo, "has taken the right fork", 0);
		pthread_mutex_lock(&philo->data->forks[left]);
		action(philo, "has taken the left fork", 0);
	}
	return (1);
}

void	put_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id;
	right = philo->id + 1;
	if (right == philo->data->amount)
		right = 0;
	pthread_mutex_unlock(&philo->data->forks[left]);
	pthread_mutex_unlock(&philo->data->forks[right]);
}

int	someone_died(t_philo *philo)
{
	int	diff;

	diff = get_time(philo->time) - philo->last_time_philo_ate;
	if (diff >= philo->time_to_die)
	{
		pthread_mutex_lock(&philo->data->action);
		if (philo->data->all_alive)
		{
			printf("%i %d %s\n", get_time(philo->time), philo->id, "died");
			philo->data->all_alive = 0;
		}
		pthread_mutex_unlock(&philo->data->action);
		put_forks(philo);
		return (1);
	}
	return (0);
}

int	is_all_alive(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->action);
	if (!philo->data->all_alive)
	{
		pthread_mutex_unlock(&philo->data->action);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->action);
	return (1);
}

void	*philo_func(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		action(philo, "is thinking", 1);
	while (1)
	{
		if (!is_all_alive(philo) || !take_forks(philo) || someone_died(philo))
			break ;
			
		philo->last_time_philo_ate = get_time(philo->time);
		action(philo, "is eating", philo->time_to_eat);
		philo->has_eaten++;
		put_forks(philo);
		action(philo, "is sleeping", philo->time_to_sleep);
		if (philo->has_eaten == philo->must_eat)
			break ;
		action(philo, "is thinking", 1);
	}
	return (NULL);
}

void *monitor_func(void *arg)
{
    t_philo **philos = (t_philo **)arg;
    int i;
    long now;

    while (1)
    {
        i = 0;
        while (philos[i])
        {
            pthread_mutex_lock(&philos[i]->data->action);
            if (!philos[i]->data->all_alive)
            {
                pthread_mutex_unlock(&philos[i]->data->action);
                return NULL;
            }
            now = get_time(philos[i]->time);
            if ((now - philos[i]->last_time_philo_ate) >= philos[i]->time_to_die)
            {
                printf("%li %d died\n", now, philos[i]->id);
                philos[i]->data->all_alive = 0;
                pthread_mutex_unlock(&philos[i]->data->action);
                return NULL;
            }
            pthread_mutex_unlock(&philos[i]->data->action);
            i++;
        }
        ft_usleep(1);
    }
    return NULL;
}

int	wrong_args(int ac, char **av)
{
	if (ft_atoi(av[1]) <= 0 || ft_atoi(av[1]) > 200)
		return (1);
	if (ft_atoi(av[2]) < 60 || ft_atoi(av[3]) < 60 || ft_atoi(av[4]) < 60)
		return (1);
	if (ac == 6 && ft_atoi(av[5]) <= 0)
		return (1);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;
	t_philo	**philos;

	if (ac < 5 || ac > 6 || wrong_args(ac, av))
	{
		printf("you stupid bitch, put right arguments\n");
		return (1);
	}
	data = init_data(ft_atoi(av[1]));
	philos = init_philos(ac, av, &data);
	t_run(philos, philo_func, monitor_func, data.amount);
	ft_free(philos, &data);
	return (0);
}
