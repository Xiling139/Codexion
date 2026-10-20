/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:52:12 by zhewu             #+#    #+#             */
/*   Updated: 2026/10/20 16:38:45 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_logs(t_hub *hub, int log_type, int tid)
{
	long long	time_ms;

	pthread_mutex_lock(&hub->p_mutex);
	pthread_mutex_lock(&hub->signal_mutex);
	if (hub->termination_signal == 1)
	{
		pthread_mutex_unlock(&hub->p_mutex);
		pthread_mutex_unlock(&hub->signal_mutex);
		return ;
	}
	pthread_mutex_unlock(&hub->signal_mutex);
	time_ms = gettime_ms(hub->start_time);
	if (log_type == 0)
		printf("%lld %d has taken a dongle\n", time_ms, tid);
	if (log_type == 1)
		printf("%lld %d is compiling\n", time_ms, tid);
	if (log_type == 2)
		printf("%lld %d is debugging\n", time_ms, tid);
	if (log_type == 3)
		printf("%lld %d is refactoring\n", time_ms, tid);
	pthread_mutex_unlock(&hub->p_mutex);
}

long long	gettime_ms(struct timeval origin)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		return (-1);
	}
	return ((tv.tv_sec - origin.tv_sec) * 1000LL + (tv.tv_usec - origin.tv_usec)
		/ 1000);
}
