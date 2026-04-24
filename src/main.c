/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhenming <zhewu@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:35:11 by zhewu             #+#    #+#             */
/*   Updated: 2026/04/24 17:35:20 by zhenming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*send_msg(void *arg)
{
	printf("%s\n", (char *)arg);
	return (NULL);
}

int	check_args(char *argv[])
{
	int		i;
	char	*scheduler;

	i = 1;
	while (i < 8)
	{
		if (!is_number(argv[i]))
			return (argument_type_error(i));
		i++;
	}
	scheduler = str_to_upper(argv[8]);
	if (scheduler == NULL)
		return (-1);
	if (strcmp("FIFO", scheduler) != 0 && strcmp("EDF", scheduler) != 0)
	{
		free(scheduler);
		return (argument_type_error(8));
	}
	free(scheduler);
	return (0);
}

t_config	setup_config(char *argv[])
{
	t_config	config;
	char		*scheduler;

	config.number_of_coders = atoi(argv[1]);
	config.time_to_burnout = atoi(argv[2]);
	config.time_to_compile = atoi(argv[3]);
	config.time_to_debug = atoi(argv[4]);
	config.time_to_refactor = atoi(argv[5]);
	config.number_of_compiles_required = atoi(argv[6]);
	config.dongle_cooldown = atoi(argv[7]);
	scheduler = str_to_upper(argv[8]);
	if (scheduler == NULL)
	{
		config.scheduler = -1;
		return (config);
	}
	if (strcmp("FIFO", scheduler) == 0)
		config.scheduler = 0;
	else if (strcmp("EDF", scheduler) == 0)
		config.scheduler = 1;
	free(scheduler);
	return (config);
}

int	main(int argc, char *argv[])
{
	pthread_t	thread1;
	pthread_t	thread2;
	t_config	config;

	if (argc != 9)
		return (argument_count_error(argc));
	if (check_args(argv) == -1)
		return (-1);
	config = setup_config(argv);
	if (config.scheduler == -1)
		return (-1);
	pthread_create(&thread1, NULL, send_msg, (void *)"Thread1 is running.");
	pthread_create(&thread2, NULL, send_msg, (void *)"Thread2 is running.");
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	printf("Both threads end\n");
	return (0);
}
