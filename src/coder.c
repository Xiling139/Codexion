/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:37:20 by zhewu             #+#    #+#             */
/*   Updated: 2026/05/30 15:37:12 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	grab_dongle(t_hub *hub, int tid)
{
	long long	time_ms;
	int			size;
	int			grabbed;

	grabbed = 0;
	size = hub->config.number_of_coders;
	if (dongle_available(hub, tid - 1))
	{
		time_ms = gettime_ms(hub->start_time);
		hub->dongles[tid - 1].available = false;
		pthread_mutex_lock(&hub->p_mutex);
		print_logs(hub, 0, tid);
		pthread_mutex_unlock(&hub->p_mutex);
		grabbed++;
	}
	if (dongle_available(hub, tid % size))
	{
		time_ms = gettime_ms(hub->start_time);
		hub->dongles[tid % size].available = false;
		pthread_mutex_lock(&hub->p_mutex);
		print_logs(hub, 0, tid);
		pthread_mutex_unlock(&hub->p_mutex);
		grabbed++;
	}
	return (grabbed);
}

void	main_loop(t_hub *hub, int tid)
{
	int	loops;
	int	grabbed;

	loops = 0;
	while (loops < hub->config.number_of_compiles_required)
	{
		grabbed = 0;
		while (grabbed < 2)
		{
			if (hub->termination_signal == 1)
				break ;
			pthread_mutex_lock(&hub->d_mutex);
			if (hub->config.scheduler == 0)
				grabbed += grab_dongle(hub, tid);
			else
				grabbed += check_dongle(hub, tid);
			pthread_mutex_unlock(&hub->d_mutex);
		}
		coder_action(hub, tid);
		loops++;
	}
	hub->burnout_time[tid - 1] = -1;
}

void	*coder(void *arg)
{
	t_coder_arg	*c_arg;
	t_hub		*hub;
	int			tid;

	c_arg = (t_coder_arg *)arg;
	tid = c_arg->thread_id + 1;
	hub = c_arg->hub;
	main_loop(hub, tid);
	return (NULL);
}
