/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_uv_coords.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:51:21 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/11 18:53:40 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

double	wrap_coord_around(double coord)
{
	coord = coord - (int) coord;
	if (coord < 0)
		coord = 1 + coord;
	return (coord);
}

static t_vec2	get_plane_uv(t_vec3 intersect, t_plane *plane)
{
	t_vec3	local_intersect;
	t_vec2	uv;

	local_intersect = vec3_sub(intersect, plane->coords);
	local_intersect = vec3_lookat(local_intersect, plane->normal);
	uv.x = wrap_coord_around(local_intersect.x);
	uv.y = wrap_coord_around(local_intersect.y);
	return (uv);
}

static t_vec2	get_cylinder_uv(t_vec3 intersect, t_cylinder *cylinder)
{
	t_vec3	local_intersect;
	t_vec2	uv;

	local_intersect = vec3_sub(intersect, cylinder->coords);
	local_intersect = vec3_lookat(local_intersect, cylinder->axis);
	uv.x = atan2(local_intersect.x, local_intersect.y) / TWO_PI + 0.5;
	uv.y = wrap_coord_around(local_intersect.z);
	return (uv);
}

static t_vec2	get_sphere_uv(t_vec3 intersect, t_vec3 obj_pos)
{
	t_vec3	local_intersect;
	t_vec2	uv;

	local_intersect = vec3_normalize(vec3_sub(intersect, obj_pos));
	uv.x = atan2(local_intersect.x, local_intersect.z) / TWO_PI + 0.5;
	uv.y = local_intersect.y * 0.5 + 0.5;
	return (uv);
}

t_vec3	get_albedo_blur(t_vec3 intersect, const t_shape *shape,
			int shape_type, double blur)
{
	t_vec2		uv;
	mlx_image_t	*img;
	t_vec3		vect;

	img = NULL;
	uv = (t_vec2) {0, 0};
	if (shape_type == SHAPE_SPHERE)
		uv = get_sphere_uv(intersect, shape->coords);
	else if (shape_type == SHAPE_PLANE || shape_type == SHAPE_DISC)
		uv = get_plane_uv(intersect, (t_plane *) shape);
	else if (shape_type == SHAPE_CYLINDER)
		uv = get_cylinder_uv(intersect, (t_cylinder *) shape);
	if (uv.x > 1. || uv.x < 0. || uv.y > 1. || uv.y < 0.)
		return (vec3(0, 0, 0));
	img = shape->texture;
	vect = get_texture_from_uv(img, uv.x, uv.y, blur / (0.25 + blur) * 1.25);
	return(vect);
}


t_vec3	get_texture_color(t_vec3 intersect, int	texture_type,
			const t_shape *shape, int shape_type)
{
	t_vec2		uv;
	mlx_image_t	*img;
	t_vec3		vect;

	img = NULL;
	uv = (t_vec2) {0, 0};
	if (shape_type == SHAPE_SPHERE)
		uv = get_sphere_uv(intersect, shape->coords);
	else if (shape_type == SHAPE_PLANE)
		uv = get_plane_uv(intersect, (t_plane *) shape);
	else if (shape_type == SHAPE_CYLINDER)
		uv = get_cylinder_uv(intersect, (t_cylinder *) shape);
	if (uv.x > 1. || uv.x < 0. || uv.y > 1. || uv.y < 0.)
		return (vec3(0, 0, 0));
	if (texture_type == ALBEDO)
		img = shape->texture;
	else if (texture_type == NORMAL_MAP)
		img = shape->normal_map;
	else if (texture_type == ROUGHNESS_MAP)
		img = shape->roughness_map;
	vect = get_texture_from_uv(img, uv.x, uv.y, 0);
	if (texture_type == NORMAL_MAP)
	{
		vect.x = -vect.x * 2 + 1;
		vect.y = vect.y * 2 - 1;
		vect.z = vect.z * 2 - 1;
	}
	return(vect);
}

double	get_rough_value(t_vec3 intersect, const t_shape *shape, int shape_type)
{
	t_vec2		uv;
	mlx_image_t	*img;
	t_vec3		vect;

	img = NULL;
	uv = (t_vec2) {0, 0};
	if (shape_type == SHAPE_SPHERE)
		uv = get_sphere_uv(intersect, shape->coords);
	else if (shape_type == SHAPE_PLANE)
		uv = get_plane_uv(intersect, (t_plane *) shape);
	else if (shape_type == SHAPE_CYLINDER)
		uv = get_cylinder_uv(intersect, (t_cylinder *) shape);
	if (uv.x > 1. || uv.x < 0. || uv.y > 1. || uv.y < 0.)
		return (0.5);
	img = shape->roughness_map;
	vect = get_texture_from_uv(img, uv.x, uv.y, 0);
	return((vect.r + vect.g + vect.b) / 3);
}
