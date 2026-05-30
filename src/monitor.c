/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:37:20 by zhewu             #+#    #+#             */
/*   Updated: 2026/05/30 15:39:02 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	count_array(int *array, int size, int target)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (i < size)
	{
		if (array[i] == target)
			count++;
		i++;
	}
	return (count);
}

t_arrmap	min_map(int *array, int size)
{
	int			i;
	t_arrmap	min;

	if (size < 0)
	{
		min.index = -1;
		min.value = INT_MAX;
		return (min);
	}
	i = 0;
	min.value = INT_MAX;
	min.index = -1;
	while (i < size)
	{
		if (array[i] < min.value && array[i] != -1)
		{
			min.index = i;
			min.value = array[i];
		}
		i++;
	}
	return (min);
}

void	broadcast_all(t_hub *hub)
{
	int	size;
	int	i;

	size = hub->config.number_of_coders;
	i = 0;
	while (i < size)
	{
		pthread_cond_broadcast(&hub->dongles[i].cv_dongle);
		i++;
	}
}

void	*monitor_run(void *arg)
{
	t_hub		*hub;
	t_arrmap	map;
	long long	time;
	int			terminate_count;

	terminate_count = 0;
	hub = ((t_coder_arg *)arg)->hub;
	while (terminate_count < hub->config.number_of_coders)
	{
		usleep(1000);
		terminate_count = count_array(hub->burnout_time,
				hub->config.number_of_coders, -1);
		time = gettime_ms(hub->start_time);
		map = min_map(hub->burnout_time, hub->config.number_of_coders);
		if (map.value < time && map.value <= hub->config.time_to_burnout)
		{
			pthread_mutex_lock(&hub->p_mutex);
			printf("%lld %d burned out\n", time, map.index + 1);
			hub->termination_signal = 1;
			pthread_mutex_unlock(&hub->p_mutex);
			broadcast_all(hub);
			break ;
		}
	}
	return (NULL);
}
