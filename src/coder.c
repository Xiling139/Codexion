/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:37:20 by zhewu             #+#    #+#             */
/*   Updated: 2026/10/07 18:09:58 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	acquire_fifo(t_hub *hub, int tid, int uid)
{
	int			ret;
	t_dongle	*dongles;

	ret = 0;
	dongles = hub->dongles;
	pthread_mutex_lock(&dongles[uid].mutex);
	if (!has_request(&dongles[uid].queue, tid))
	{
		enqueue(&dongles[uid].queue,
			(t_request){.priority = gettime_ms(hub->start_time), .tid = tid});
	}
	else if (dongle_available(hub, uid) && peek(&dongles[uid].queue).tid == tid)
	{
		dequeue(&dongles[uid].queue);
		hub->dongles[uid].available = false;
		ret = 1;
	}
	pthread_mutex_unlock(&dongles[uid].mutex);
	return (ret);
}

int	acquire(t_hub *hub, int tid, int uid, int loops)
{
	if (hub->config.scheduler == 0)
		return (acquire_fifo(hub, tid, uid));
	else
		return (acquire_edf(hub, tid, uid, loops));
	if (hub->config.scheduler == 0)
		return (acquire_fifo(hub, tid, uid));
	else
		return (acquire_edf(hub, tid, uid, loops));
}

int	grab_dongles(t_hub *hub, int tid, int loops)
{
	int	size;

	size = hub->config.number_of_coders;
	if (acquire(hub, tid, tid - 1, loops) == 0)
		return (0);
	if (acquire(hub, tid, tid % size, loops) == 0)
	{
		d_release(hub, tid - 1, false);
		return (0);
	}
	pthread_mutex_lock(&hub->p_mutex);
	print_logs(hub, 0, tid);
	print_logs(hub, 0, tid);
	pthread_mutex_unlock(&hub->p_mutex);
	return (2);
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
			if (terminated(hub))
				break ;
			pthread_mutex_lock(&hub->d_mutex);
			grabbed += grab_dongles(hub, tid, loops);
			pthread_mutex_unlock(&hub->d_mutex);
		}
		if (coder_action(hub, tid) == -1)
			break ;
		loops++;
	}
	pthread_mutex_lock(&hub->arr_mutex);
	hub->burnout_time[tid - 1] = -1;
	pthread_mutex_unlock(&hub->arr_mutex);
}

void	*coder(void *arg)
{
	t_coder_arg	*c_arg;
	t_hub		*hub;
	int			tid;
	int			size;

	c_arg = (t_coder_arg *)arg;
	tid = c_arg->thread_id + 1;
	hub = c_arg->hub;
	size = hub->config.number_of_coders;
	main_loop(hub, tid);
	return (NULL);
}
