/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_dist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:29:56 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/10 16:09:40 by lfiestas         ###   ########.fr       */
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
		if (length < ray->length && length >= 0.) // TODO square these too
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
	if (length < ray->length && length >= 0.)
	{
		ray->length = length;
		ray->shape = (t_shape *)plane;
		ray->shape_type = SHAPE_PLANE;
	}
}

// static void	min_disc_intersect_dist(
// 	t_ray *ray, const t_cylinder *cylinder, t_vec3 top, t_vec3 bot)
// {
// 	t_ray	disc_ray;
// 	t_plane	disc_top;
// 	t_plane	disc_bot;
// 	double	length;
//
// 	length = INFINITY;
// 	disc_ray = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};
//
// 	disc_top = (t_plane){.coords = top, .normal = cylinder->axis};
// 	min_plane_intersect_dist(&disc_ray, &disc_top);
// 	if (vec3_length(vec3_sub(vec3_muls(disc_ray.dir, disc_ray.length), top)) <= cylinder->radius) // TODO square these
// 		length = disc_ray.length;
//
// 	disc_bot = (t_plane){.coords = bot, .normal = cylinder->axis};
// 	min_plane_intersect_dist(&disc_ray, &disc_bot);
// 	if (vec3_length(vec3_sub(vec3_muls(disc_ray.dir, disc_ray.length), bot)) <= cylinder->radius) // TODO square these
// 		length = fmin(length, disc_ray.length);
//
// 	if (length < ray->length && length >= 0.)
// 	{
// 		ray->length = length;
// 		ray->shape = (t_shape *)cylinder;
// 		ray->shape_type = SHAPE_PLANE;
// 	}
// }

static void	min_cap_intersect_dist(
	t_ray *ray, const t_cylinder *cylinder, t_vec3 cap)
{
	double	length;
	t_plane	plane;
	t_ray	disc_ray;

	length = INFINITY;
	disc_ray = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};
	plane = (t_plane){.coords = cap, .normal = cylinder->axis};
	min_plane_intersect_dist(&disc_ray, &plane);
	t_vec3 offset = vec3_sub(vec3_muls(disc_ray.dir, disc_ray.length), cap);
	if (vec3_dot(offset, offset) <= cylinder->radius * cylinder->radius)
		length = disc_ray.length;

	if (length < ray->length && length >= 0.)
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
	//t_vec3	x;
	double	axis_dot_dir;
	//double	x_dot_axis;
	t_vec3	cap;
	t_vec3	top;
	t_vec3	bot;
	//double	top_sub_hitp_dot_axis;

	/*
	x = vec3_sub(ray->start, cylinder->coords);                    // ray to cylinder center direction
	axis_dot_dir = vec3_dot(cylinder->axis, ray->dir);             // how aligned ray dir and axis are
	x_dot_axis = vec3_dot(x, cylinder->axis);                      // how aligned ray to cylinder is with axis
	a = 1. - axis_dot_dir * axis_dot_dir;
	b = 2. * (vec3_dot(ray->dir, x) - axis_dot_dir * x_dot_axis);
	c = vec3_dot(x, x) \
		- x_dot_axis * x_dot_axis - \
		cylinder->radius * cylinder->radius;
	*/



	cap = vec3_muls(cylinder->axis, cylinder->height / 2.);
	top = vec3_add(cylinder->coords, cap);
	bot = vec3_sub(cylinder->coords, cap);

	axis_dot_dir = vec3_dot(cylinder->axis, ray->dir);
	t_vec3 rl = vec3_sub(ray->start, bot);
	//t_vec3 rl = vec3_sub(ray->start, cylinder->coords);
	double axis_dot_rl = vec3_dot(cylinder->axis, rl);

	a = 1. - axis_dot_dir * axis_dot_dir;
	b = 2. * (vec3_dot(ray->dir, rl) - vec3_dot(cylinder->axis, ray->dir) * axis_dot_rl);
	c = vec3_dot(rl, rl) - axis_dot_rl * axis_dot_rl - cylinder->radius * cylinder->radius;

	discriminant = b * b - 4 * a * c;

	if (discriminant >= 0.)
	{
		// double length1 = (-b - sqrt(discriminant)) / (2. * a);
		// double length2 = (-b + sqrt(discriminant)) / (2. * a);
		// if (length1 < 0 && length2 < 0)
		// 	return ;
		// length = fmin(fmax(0, length1), fmax(0, length2));

		// double b_term = b <= 0. ? -b + sqrt(discriminant) : -b - sqrt(discriminant);
		// double length1 = b_term / (2. * a);
		// double length2 = (2. * c) / b_term;
		// if (length1 < 0 && length2 < 0)
		// 	return ;
		// length = fmin(fmax(0, length1), fmax(0, length2));

		// top_sub_hitp_dot_axis = vec3_dot( \
		// 	vec3_sub(top, vec3_muls(ray->dir, length)), cylinder->axis);

		// if (!(0 <= top_sub_hitp_dot_axis
		// 	&& top_sub_hitp_dot_axis <= cylinder->height))
		// 	min_disc_intersect_dist(ray, cylinder, top, bot);
		// else if (length < ray->length && length >= 0.)
		// {
		// 	ray->length = length;
		// 	ray->shape = (t_shape *)cylinder;
		// 	ray->shape_type = SHAPE_CYLINDER;
		// }

		bool valid1 = true;
		bool valid2 = true;
		double b_term = b <= 0. ? -b + sqrt(discriminant) : -b - sqrt(discriminant);
		double hit_far = b_term / (2. * a);
		double hit_near = (2. * c) / b_term;

		t_vec3 hitp1 = vec3_add(ray->start, vec3_muls(ray->dir, hit_near));
		t_vec3 hitp2 = vec3_add(ray->start, vec3_muls(ray->dir, hit_far));
		double hoffset1 = vec3_dot(vec3_sub(top, hitp1), cylinder->axis);
		double hoffset2 = vec3_dot(vec3_sub(top, hitp2), cylinder->axis);

		if (hit_near < 0. || hoffset1 <= 0. || hoffset1 > cylinder->height)
			valid1 = false;
		if (hit_far < 0. || hoffset2 <= 0. || hoffset2 > cylinder->height)
			valid2 = false;

		if (!valid1 && !valid2)
		{
			if (axis_dot_rl <= 0.)
				min_cap_intersect_dist(ray, cylinder, top);
			else if (axis_dot_rl >= cylinder->height)
				min_cap_intersect_dist(ray, cylinder, bot);
			// else inside. // TODO handle later
		}
		else if (valid1 ^ valid2)
		{
			if (valid2)
			{
				hitp1 = hitp2; // TODO do we need this??
				hoffset1 = hoffset2; // TODO do we need this??
				hit_near = hit_far;
			}
			t_ray	disc_near = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};
			t_ray	disc_far  = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};
			min_cap_intersect_dist(&disc_near, cylinder, top);
			min_cap_intersect_dist(&disc_far, cylinder, bot);
			// if (!isinf(disc_near.length))
			// {
			// 	if (!isinf(disc_far.length) && disc_far.length < disc_near.length)
			// 		disc_near = disc_far;
			// }
			// else if (!isinf(disc_far.length))
			// {
			// 	disc_near.length = hit_far;
			// 	disc_near = disc_far
			// }
			double cap_length = fmin(fmax(0, disc_near.length), fmax(0, disc_far.length));
			length = fmin(cap_length, hit_near);

			if (length < ray->length && length >= 0.)
			{
				ray->length = length;
				ray->shape = (t_shape *)cylinder;
				if (cap_length < hit_near)
					ray->shape_type = SHAPE_PLANE;
				else
					ray->shape_type = SHAPE_CYLINDER;
			}
		}
		else
		{
			length = fmin(hit_near, hit_far);
			if (length < ray->length && length >= 0.)
			{
				ray->length = length;
				ray->shape = (t_shape *)cylinder;
				ray->shape_type = SHAPE_CYLINDER;
			}
		}
	}
}

void	get_shape_intersect_dist(t_minirt *m, t_ray *ray, const t_shape *skip)
{
	size_t	i;

	mrt_debug(m);

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
