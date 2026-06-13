/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coderedf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:30:19 by zhewu             #+#    #+#             */
/*   Updated: 2026/06/13 13:28:58 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	is_prioritized(t_hub *hub, int index, int adjacent)
{
	if (hub->config.number_of_coders == 1)
		return (true);
	if (hub->burnout_time[index] == hub->burnout_time[adjacent])
	{
		if (hub->burnout_time[adjacent] == -1)
			return (true);
		else
			return (index % 2 == 1 || adjacent % 2 == 0);
	}
	if (hub->burnout_time[index] > hub->burnout_time[adjacent]
		&& hub->burnout_time[adjacent] != -1)
		return (false);
	return (true);
}

void	grab_dongle_edf(t_hub *hub, int tid, int index)
{
	long long	time_ms;
	long long	cd_end_ms;
	long long	wait_time;

	cd_end_ms = hub->dongles[index].t_unlock_ms;
	if (!dongle_available(hub, index))
	{
		time_ms = gettime_ms(hub->start_time);
		wait_time = cd_end_ms - time_ms;
		if (wait_time > 0)
		{
			usleep(wait_time);
			return (grab_dongle_edf(hub, tid, index));
		}
	}
	time_ms = gettime_ms(hub->start_time);
	hub->dongles[index].available = false;
	pthread_mutex_lock(&hub->p_mutex);
	print_logs(hub, 0, tid);
	pthread_mutex_unlock(&hub->p_mutex);
}

int	check_dongle(t_hub *hub, int tid)
{
	int	size;
	int	grabbed;

	grabbed = 0;
	size = hub->config.number_of_coders;
	if (dongle_available(hub, tid - 1))
	{
		if (!is_prioritized(hub, tid - 1, (size + tid - 2) % size))
			pthread_cond_wait(&hub->dongles[tid - 1].cv_dongle, &hub->d_mutex);
		grab_dongle_edf(hub, tid, tid - 1);
		grabbed++;
	}
	if (dongle_available(hub, tid % size))
	{
		if (!is_prioritized(hub, tid - 1, tid % size))
			pthread_cond_wait(&hub->dongles[tid % size].cv_dongle,
				&hub->d_mutex);
		grab_dongle_edf(hub, tid, tid % size);
		grabbed++;
	}
	return (grabbed);
}
