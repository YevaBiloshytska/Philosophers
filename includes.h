/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybiloshy <ybiloshy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 21:14:25 by ybiloshy          #+#    #+#             */
/*   Updated: 2025/08/31 00:03:48 by ybiloshy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>  //pthread_..
#include <stdio.h>    //printf
#include <stdlib.h>   //malloc, free
#include <sys/time.h> //gettimeofday
#include <unistd.h>   //usleep

typedef void		*(*t_fn_ptr)(void *);

typedef struct s_data
{
	int				all_alive;
	int				all_ate;
	int				amount;
	pthread_mutex_t	action;
	pthread_mutex_t	*forks;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				has_eaten;
	int				must_eat;
	struct timeval	time;
	int				last_time_philo_ate;
	t_data			*data;
}					t_philo;

void				ft_usleep(int time);
t_data				init_data(int amount);
int					get_time(struct timeval time);
int					ft_atoi(const char *n);
void				ft_free(t_philo **philos, t_data *data);
void				t_run(t_philo **philos, t_fn_ptr process, t_fn_ptr monitor, size_t n);
t_philo				**init_philos(int ac, char **av, t_data *data);
