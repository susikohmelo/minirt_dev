/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_dist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:29:56 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/07 12:39:51 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>


void	min_sphere_intersect_dist(t_ray *ray, const t_sphere *sphere)
{
	t_vec3	lstart;
	double	b;
	double	c;
	double	discriminant;
	double	length;

	lstart = vec3_sub(ray->start, sphere->coords);
	b = 2 * vec3_dot(ray->dir, lstart);
	c = vec3_dot(lstart, lstart) - sphere->radius * sphere->radius;
	discriminant = b * b - 4 * 1 * c;
	if (discriminant >= 0.)
	{
		length = (-b - sqrt(discriminant)) / (2. * 1);
		if (length <= ray->length && length >= 0.)
		{
			ray->length = length;
			ray->shape = (t_shape *)sphere;
			ray->shape_type = SHAPE_SPHERE;
		}
	}
}

void	min_plane_intersect_dist(t_ray *ray, const t_plane *plane)
{
	double	length;

	length = -vec3_dot(vec3_sub(ray->start, plane->coords), plane->normal) / \
		vec3_dot(ray->dir, plane->normal);
	if (length <= ray->length && length >= 0.)
	{
		ray->length = length;
		ray->shape = (t_shape *)plane;
		ray->shape_type = SHAPE_PLANE;
	}
}

// void	min_cylinder_intersect_dist(t_ray ray, t_cylinder cylinder)
// {
// 	(void)ray;
// 	(void)cylinder;
// 	return (INFINITY);
// }
//
