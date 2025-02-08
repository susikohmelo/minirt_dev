/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_dist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:29:56 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/08 17:11:01 by lfiestas         ###   ########.fr       */
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
		length = (-b - sqrt(discriminant)) / (2. * 1); // TODO square this and calculate sqrt() lazily
		if (length <= ray->length && length >= 0.) // TODO square these too
		{
			ray->length = length; // and the sqrt() would go here
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

static void	min_disc_intersect_dist(
	t_ray *ray, const t_cylinder *cylinder, t_vec3 top, t_vec3 bot)
{
	t_ray	disc_ray;
	t_plane	disc_top;
	t_plane	disc_bot;
	double	length;

	length = INFINITY;
	disc_ray = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};

	disc_top = (t_plane){.coords = top, .normal = cylinder->axis};
	min_plane_intersect_dist(&disc_ray, &disc_top);
	if (vec3_length(vec3_sub(vec3_muls(disc_ray.dir, disc_ray.length), top)) <= cylinder->radius) // TODO square these
		length = disc_ray.length;

	disc_bot = (t_plane){.coords = bot, .normal = cylinder->axis};
	min_plane_intersect_dist(&disc_ray, &disc_bot);
	if (vec3_length(vec3_sub(vec3_muls(disc_ray.dir, disc_ray.length), bot)) <= cylinder->radius) // TODO square these
		length = fmin(length, disc_ray.length);

	if (length <= ray->length && length >= 0.)
	{
		ray->length = length;
		ray->shape = (t_shape *)cylinder;
		ray->shape_type = SHAPE_PLANE;
	}
}

void	min_cylinder_intersect_dist(t_ray *ray, const t_cylinder *cylinder)
{
	double	length;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	t_vec3	x;
	double	dir_dot_axis;
	double	x_dot_axis;
	t_vec3	cap;
	t_vec3	top;
	t_vec3	bot;
	double	top_sub_hitp_dot_axis;

	x = vec3_sub(ray->start, cylinder->coords);
	dir_dot_axis = vec3_dot(ray->dir, cylinder->axis);
	x_dot_axis = vec3_dot(x, cylinder->axis);
	a = 1. - dir_dot_axis * dir_dot_axis;
	b = 2. * (vec3_dot(ray->dir, x) - dir_dot_axis * x_dot_axis);
	c = vec3_dot(x, x) \
		- x_dot_axis * x_dot_axis - \
		cylinder->radius * cylinder->radius;
	discriminant = b * b - 4 * a * c;

	if (discriminant >= 0.)
	{
		cap = vec3_muls(cylinder->axis, cylinder->height / 2.);
		top = vec3_add(cylinder->coords, cap);
		bot = vec3_sub(cylinder->coords, cap);

		length = (-b - sqrt(discriminant)) / (2. * a);

		top_sub_hitp_dot_axis = vec3_dot( \
			vec3_sub(top, vec3_muls(ray->dir, length)), cylinder->axis);

		if (!(0 <= top_sub_hitp_dot_axis
			&& top_sub_hitp_dot_axis <= vec3_length(vec3_sub(top, bot)))) // TODO square this
			min_disc_intersect_dist(ray, cylinder, top, bot);
		else if (length <= ray->length && length >= 0.)
		{
			ray->length = length;
			ray->shape = (t_shape *)cylinder;
			ray->shape_type = SHAPE_CYLINDER;
		}
	}
}

void	get_shape_intersect_dist(t_minirt *m, t_ray *ray, const t_shape *skip)
{
	size_t	i;

	i = (size_t) - 1;
	while (++i < m->spheres_length)
		if ((t_shape *)&m->spheres[i] != skip)
			min_sphere_intersect_dist(ray, &m->spheres[i]);
	i = (size_t) - 1;
	while (++i < m->planes_length)
		if ((t_shape *)&m->planes[i] != skip)
			min_plane_intersect_dist(ray, &m->planes[i]);
	i = (size_t) - 1;
	while (++i < m->cylinders_length)
		if ((t_shape *)&m->cylinders[i] != skip)
			min_cylinder_intersect_dist(ray, &m->cylinders[i]);
}
