/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coderedf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:30:19 by zhewu             #+#    #+#             */
/*   Updated: 2026/09/25 16:05:38 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	calculate_priority(t_hub *hub, int tid, int loops)
{
	int			first_deadline;
	int			index;
	long long	burn_out_time;

	first_deadline = hub->config.time_to_burnout;
	index = tid - 1;
	burn_out_time = hub->burnout_time[index];
	if (tid % 2 == 1 && burn_out_time == first_deadline)
	{
		usleep(2000);
		burn_out_time++;
	}
	if (loops == 0)
	{
		burn_out_time -= hub->config.time_to_compile;
	}
	return (burn_out_time);
}

int	acquire_edf(t_hub *hub, int tid, int uid, int loops)
{
	int			ret;
	long long	priority;
	t_dongle	*dongles;

	ret = 0;
	dongles = hub->dongles;
	pthread_mutex_lock(&dongles[uid].mutex);
	if (!has_request(&dongles[uid].queue, tid))
	{
		priority = calculate_priority(hub, tid, loops);
		enqueue(&dongles[uid].queue, (t_request){.priority = priority,
			.tid = tid});
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
