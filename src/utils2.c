/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:52:12 by zhewu             #+#    #+#             */
/*   Updated: 2026/05/30 13:48:02 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_logs(t_hub *hub, int log_type, int tid)
{
	long long	time_ms;

	if (hub->termination_signal == 1)
		return ;
	time_ms = gettime_ms(hub->start_time);
	if (log_type == 0)
		printf("%lld %d has taken a dongle\n", time_ms, tid);
	if (log_type == 1)
		printf("%lld %d is compiling\n", time_ms, tid);
	if (log_type == 2)
		printf("%lld %d is debugging\n", time_ms, tid);
	if (log_type == 3)
		printf("%lld %d is refactoring\n", time_ms, tid);
}

long long	gettime_ms(struct timeval origin)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec - origin.tv_sec) * 1000LL + (tv.tv_usec - origin.tv_usec)
		/ 1000);
}

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
