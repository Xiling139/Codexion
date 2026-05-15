/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhewu <zhewu@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:52:12 by zhewu             #+#    #+#             */
/*   Updated: 2026/05/11 18:15:08 by zhewu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long gettime_ms(struct timeval origin)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec - origin.tv_sec) * 1000LL + 
           (tv.tv_usec - origin.tv_usec) / 1000;
}