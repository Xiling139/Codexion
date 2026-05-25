/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 11:37:20 by zhewu             #+#    #+#             */
/*   Updated: 2026/05/25 18:01:58 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_run(void *arg)
{
	t_hub		*hub;
	long long	time;
	int			i;
	int			terminate_count;

	terminate_count = 0;
	i = 0;
	hub = ((t_coder_arg *)arg)->hub;
	while (terminate_count < hub->config.number_of_coders)
	{
		if (hub->burnout_time[i] == -1)
			terminate_count++;
		else
			terminate_count = 0;
		time = gettime_ms(hub->start_time);
		if (time >= hub->burnout_time[i] && hub->burnout_time[i] != -1)
		{
			printf("%lld %d burned out\n", time, i + 1);
			break ;
		}
		i = (i + 1) % hub->config.number_of_coders;
		usleep(5000 / hub->config.number_of_coders);
	}
	return (NULL);
}
