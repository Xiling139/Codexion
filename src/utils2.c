/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:52:12 by zhewu             #+#    #+#             */
/*   Updated: 2026/05/15 17:18:12 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	gettime_ms(struct timeval origin)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec - origin.tv_sec) * 1000LL + (tv.tv_usec - origin.tv_usec)
		/ 1000);
}

bool	compile_available(t_hub *hub, int tid)
{
	long long	time;
	int			ldongle;
	int			rdongle;

	ldongle = tid - 1;
	rdongle = tid % hub->config.number_of_coders;
	time = gettime_ms(hub->start_time);
	if (!hub->dongles[ldongle].available
		|| hub->dongles[ldongle].t_unlock_ms > time)
		return (false);
	if (!hub->dongles[rdongle].available
		|| hub->dongles[rdongle].t_unlock_ms > time)
		return (false);
	return (true);
}

void	d_release(t_hub *hub, int tid)
{
	long long	time;
	long long	cd_time_ms;
	int			size;

	size = hub->config.number_of_coders;
	cd_time_ms = hub->config.dongle_cooldown;
	time = gettime_ms(hub->start_time);
	hub->dongles[tid - 1].available = true;
	hub->dongles[tid - 1].t_unlock_ms = time + cd_time_ms;
	time = gettime_ms(hub->start_time);
	hub->dongles[tid % size].available = true;
	hub->dongles[tid % size].t_unlock_ms = time + cd_time_ms;
}
