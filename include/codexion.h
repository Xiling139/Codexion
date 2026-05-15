/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 16:09:31 by zhenming          #+#    #+#             */
/*   Updated: 2026/05/15 16:15:34 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_config
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	int				scheduler;
}					t_config;

typedef struct s_dongle
{
	bool			available;
	long long		t_unlock_ms;
	int				id;
}					t_dongle;

typedef struct s_hub
{
	struct timeval	start_time;
	t_dongle		*dongles;
	t_config		config;
	pthread_t		*coders;
	pthread_mutex_t	d_mutex;
	pthread_cond_t	cv;
}					t_hub;

typedef struct s_coder_arg
{
	int				thread_id;
	t_hub			*hub;
}					t_coder_arg;

// Core functions
int					setup(t_config config);

// Thread functions
void				*coder(void *arg);
void				*monitor_run(void *arg);

// Errors
int					argument_count_error(int argc);
int					argument_type_error(int arg_num);
int					empty_argument_error(void);

// Util functions
bool				is_number(char *str);
char				*str_to_upper(char *str);

// Thread utils
long long			gettime_ms(struct timeval origin);
bool				compile_available(t_hub *hub, int tid);
void				d_release(t_hub *hub, int tid);

#endif
