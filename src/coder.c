/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:37:20 by zhewu             #+#    #+#             */
/*   Updated: 2026/05/11 18:30:57 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	grab_dongle(t_hub *hub, int id)
{
	struct timeval	tv;
	time_t			secdiff;
	suseconds_t		usecdiff;
	time_t			timediff;

	if (hub->free_dongles > 0)
	{
		gettimeofday(&tv, NULL);
		secdiff = tv.tv_sec - hub->start_time.tv_sec;
		usecdiff = tv.tv_usec - hub->start_time.tv_usec;
		timediff = secdiff * 1000 + usecdiff / 1000;
		printf("%ld %d has taken a dongle\n", timediff, id);
		hub->free_dongles--;
	}
}

void	release_dongle(t_hub *hub, int id)
{
	struct timeval	tv;
	suseconds_t		time;

	gettimeofday(&tv, NULL);
	time = (tv.tv_usec - hub->start_time.tv_usec) / 1000;
	printf("%ld %d has released a dongle\n", time, id);
	hub->free_dongles++;
}

void	main_loop(t_hub *hub, int tid)
{
	int	loops;

	loops = 0;
	while (loops < hub->config.number_of_compiles_required)
	{
		pthread_mutex_lock(&hub->d_mutex);
		grab_dongle(hub, tid);
		grab_dongle(hub, tid);
		pthread_mutex_unlock(&hub->d_mutex);
		usleep(hub->config.time_to_compile * 1000);
		pthread_mutex_lock(&hub->d_mutex);
		release_dongle(hub, tid);
		release_dongle(hub, tid);
		pthread_mutex_unlock(&hub->d_mutex);
		loops++;
	}
}

void	*coder(void *arg)
{
	t_coder_arg	*c_arg;
	t_hub		*hub;
	int			tid;

	c_arg = (t_coder_arg *)arg;
	tid = c_arg->thread_id + 1;
	hub = c_arg->hub;
	printf("Thread ID: %d initialized.\n", tid);
	main_loop(hub, tid);
	return (NULL);
}
