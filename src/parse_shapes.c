/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shapes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 20:00:45 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/03 13:14:02 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

void	parse_sphere(t_minirt *m, const char *line);
void	parse_plane(t_minirt *m, const char *line);
void	parse_cylinder(t_minirt *m, const char *line);
