/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 16:09:31 by zhenming          #+#    #+#             */
/*   Updated: 2026/06/13 11:45:33 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
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
	pthread_cond_t	cv_dongle;
}					t_dongle;

typedef struct s_hub
{
	int				termination_signal;
	int				*burnout_time;
	struct timeval	start_time;
	t_dongle		*dongles;
	t_config		config;
	pthread_t		*coders;

	// Mutex for dongle
	pthread_mutex_t	d_mutex;

	// Mutex for printf
	pthread_mutex_t	p_mutex;

	// Cond Variable for EDF queue
	pthread_cond_t	cv;
}					t_hub;

typedef struct s_coder_arg
{
	int				thread_id;
	t_hub			*hub;
}					t_coder_arg;

typedef struct s_arrmap
{
	int				index;
	int				value;
}					t_arrmap;

// Core functions
int					setup(t_config config);

// Thread functions
void				*coder(void *arg);
void				*monitor_run(void *arg);
void				coder_action(t_hub *hub, int tid);

// EDF scheduler
int					check_dongle(t_hub *hub, int tid);

// Errors
int					argument_count_error(int argc);
int					argument_type_error(int arg_num);
int					empty_argument_error(void);

// Util functions
bool				is_number(char *str);
bool				overflow(char *nbr);
char				*str_to_upper(char *str);
int					*init_int_arr(t_config config);

// Thread utils
long long			gettime_ms(struct timeval origin);
bool				dongle_available(t_hub *hub, int index);
void				d_release(t_hub *hub, int tid);
void				wait_threads(t_hub *hub);
void				print_logs(t_hub *hub, int log_type, int tid);

#endif
