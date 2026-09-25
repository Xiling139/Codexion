/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 14:58:23 by zhewu             #+#    #+#             */
/*   Updated: 2026/09/25 16:04:08 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	dongle_available(t_hub *hub, int index)
{
	long long	time;
	t_dongle	dongle;

	dongle = hub->dongles[index];
	time = gettime_ms(hub->start_time);
	if (!dongle.available || dongle.t_unlock_ms > time)
		return (false);
	return (true);
}

void	d_release(t_hub *hub, int uid, bool used)
{
	long long	time;
	long long	cd_time_ms;

	time = gettime_ms(hub->start_time);
	cd_time_ms = hub->config.dongle_cooldown;
	pthread_mutex_lock(&hub->dongles[uid].mutex);
	hub->dongles[uid].available = true;
	if (used)
		hub->dongles[uid].t_unlock_ms = time + cd_time_ms;
	pthread_mutex_unlock(&hub->dongles[uid].mutex);
}

void	release_dongles(t_hub *hub, int tid)
{
	int	size;

	size = hub->config.number_of_coders;
	d_release(hub, tid - 1, true);
	d_release(hub, tid % size, true);
}

void	wait_threads(t_hub *hub)
{
	int	i;
	int	size;

	i = 0;
	size = hub->config.number_of_coders;
	while (i <= size)
	{
		pthread_join(hub->coders[i], NULL);
		i++;
	}
}
