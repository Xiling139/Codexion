/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coderedf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:30:19 by zhewu             #+#    #+#             */
/*   Updated: 2026/10/08 15:57:07 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	calculate_priority(t_hub *hub, int tid, int loops)
{
	int			index;
	long long	burn_out_time;

	index = tid - 1;
	pthread_mutex_lock(&hub->arr_mutex);
	burn_out_time = hub->burnout_time[index];
	pthread_mutex_unlock(&hub->arr_mutex);
	if (loops == 0)
	{
		burn_out_time -= hub->config.time_to_compile;
	}
	return (burn_out_time);
}

void	update_request(t_queue *pq, int tid, long long priority)
{
	int	i;

	i = 0;
	while (i < pq->size)
	{
		if (pq->items[i].tid == tid)
			pq->items[i].priority = priority;
		i++;
	}
}

int	acquire_edf(t_hub *hub, int tid, int uid, int loops)
{
	int			ret;
	long long	priority;
	t_dongle	*dongles;
	t_request	request;

	ret = 0;
	dongles = hub->dongles;
	priority = calculate_priority(hub, tid, loops);
	if (!has_request(&dongles[uid].queue, tid))
		enqueue(&dongles[uid].queue, (t_request){.priority = priority,
			.tid = tid});
	else
		update_request(&dongles[uid].queue, tid, priority);
	request = peek(&dongles[uid].queue);
	if (dongle_available(hub, uid) && request.tid == tid)
	{
		hub->dongles[uid].available = false;
		ret = 1;
	}
	return (ret);
}
