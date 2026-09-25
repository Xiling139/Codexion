/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 16:09:31 by zhenming          #+#    #+#             */
/*   Updated: 2026/09/25 16:03:52 by zhewu            ###   ########.fr       */
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

// Priority Queue and Request
typedef struct s_request
{
	int				tid;
	long			priority;
}					t_request;

typedef struct s_queue
{
	t_request		*items;
	int				size;
}					t_queue;

typedef struct s_dongle
{
	bool			available;
	long long		t_unlock_ms;
	int				id;

	int				holder;
	pthread_mutex_t	mutex;
	t_queue			queue;
}					t_dongle;

typedef struct s_hub
{
	int				termination_signal;
	int				*burnout_time;
	struct timeval	start_time;
	t_dongle		*dongles;
	t_config		config;
	pthread_t		*coders;

	// Mutex for printf
	pthread_mutex_t	p_mutex;

	// Additional mutexes
	pthread_mutex_t	arr_mutex;
	pthread_mutex_t	signal_mutex;

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

// Priority Queue
void				enqueue(t_queue *pq, t_request request);
t_request			dequeue(t_queue *pq);
t_request			peek(t_queue *pq);
void				swap(t_request *a, t_request *b);
void				queue_init(t_queue *pq);
void				queue_free(t_queue *pq);
bool				has_request(t_queue *pq, int tid);

// Core functions
int					setup(t_config config);

// Thread functions
void				*coder(void *arg);
void				*monitor_run(void *arg);
int					coder_action(t_hub *hub, int tid);

// EDF scheduler
int					acquire_edf(t_hub *hub, int tid, int uid, int loops);

// Errors
int					argument_count_error(int argc);
int					argument_type_error(int arg_num);
int					empty_argument_error(void);
int					integer_overflow_error(void);

// Util functions
bool				terminated(t_hub *hub);
bool				is_number(char *str);
bool				overflow(char *nbr);
char				*str_to_upper(char *str);
int					*init_int_arr(t_config config);

// Thread utils
long long			gettime_ms(struct timeval origin);
bool				dongle_available(t_hub *hub, int index);
void				d_release(t_hub *hub, int uid, bool used);
void				release_dongles(t_hub *hub, int tid);
void				wait_threads(t_hub *hub);
void				print_logs(t_hub *hub, int log_type, int tid);

#endif
