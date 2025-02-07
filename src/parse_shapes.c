/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shapes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 20:00:45 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/07 11:36:00 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	parse_sphere(t_minirt *m, const char *line)
{
	t_sphere	sphere; // TODO diameter for readability

	line = trim_left(line);
	line = parse_float(m, &sphere.coords.x, line, ',');
	line = parse_float(m, &sphere.coords.y, line, ',');
	line = parse_float(m, &sphere.coords.z, line, ' ');
	line = parse_float(m, &sphere.radius, line, ' ');
	sphere.radius /= 2.;
	line = parse_float(m, &sphere.color.r, line, ',');
	assert_range(m, vec3(sphere.color.r, 0, 255), "Sphere red component");
	line = parse_float(m, &sphere.color.g, line, ',');
	assert_range(m, vec3(sphere.color.g, 0, 255), "Sphere green component");
	line = parse_float(m, &sphere.color.b, line, -1);
	assert_range(m, vec3(sphere.color.b, 0, 255), "Sphere blue component");
	sphere.color = vec3_divs(sphere.color, 255);
	line = parse_texture(m, line, (t_shape *) &sphere);
	m->spheres[m->spheres_length++] = sphere;
}

void	parse_plane(t_minirt *m, const char *line)
{
	t_plane	plane;

	line = trim_left(line);
	line = parse_float(m, &plane.coords.x, line, ',');
	line = parse_float(m, &plane.coords.y, line, ',');
	line = parse_float(m, &plane.coords.z, line, ' ');
	line = parse_float(m, &plane.normal.x, line, ',');
	assert_range(m, vec3(plane.normal.x, -1, 1), "Plane normal x component");
	line = parse_float(m, &plane.normal.y, line, ',');
	assert_range(m, vec3(plane.normal.y, -1, 1), "Plane normal y component");
	line = parse_float(m, &plane.normal.z, line, ' ');
	assert_range(m, vec3(plane.normal.z, -1, 1), "Plane normal z component");
	plane.normal = expect_normalized(plane.normal, "camera orientation");
	plane.normal = vec3_normalize(plane.normal);
	line = parse_float(m, &plane.color.r, line, ',');
	assert_range(m, vec3(plane.color.r, 0, 255), "Plane red component");
	line = parse_float(m, &plane.color.g, line, ',');
	assert_range(m, vec3(plane.color.g, 0, 255), "Plane green component");
	line = parse_float(m, &plane.color.b, line, -1);
	assert_range(m, vec3(plane.color.b, 0, 255), "Plane blue component");
	plane.color = vec3_divs(plane.color, 255);
	line = parse_texture(m, line, (t_shape *) &plane);
	m->planes[m->planes_length++] = plane;
}

void	parse_cylinder(t_minirt *m, const char *line)
{
	t_cylinder	cylinder;

	line = trim_left(line);
	line = parse_float(m, &cylinder.coords.x, line, ',');
	line = parse_float(m, &cylinder.coords.y, line, ',');
	line = parse_float(m, &cylinder.coords.z, line, ' ');
	line = parse_float(m, &cylinder.axis.x, line, ',');
	assert_range(m, vec3(cylinder.axis.x, -1, 1), "Cylinder axis x component");
	line = parse_float(m, &cylinder.axis.y, line, ',');
	assert_range(m, vec3(cylinder.axis.y, -1, 1), "Cylinder axis y component");
	line = parse_float(m, &cylinder.axis.z, line, ' ');
	assert_range(m, vec3(cylinder.axis.z, -1, 1), "Cylinder axis z component");
	cylinder.axis = expect_normalized(cylinder.axis, "cylinder axis");
	cylinder.axis = vec3_normalize(cylinder.axis);
	line = parse_float(m, &cylinder.diameter, line, ' ');
	line = parse_float(m, &cylinder.height, line, ' ');
	line = parse_float(m, &cylinder.color.r, line, ',');
	assert_range(m, vec3(cylinder.color.r, 0, 255), "Cylinder red component");
	line = parse_float(m, &cylinder.color.g, line, ',');
	assert_range(m, vec3(cylinder.color.g, 0, 255), "Cylinder green component");
	line = parse_float(m, &cylinder.color.b, line, -1);
	assert_range(m, vec3(cylinder.color.b, 0, 255), "Cylinder blue component");
	cylinder.color = vec3_divs(cylinder.color, 255);
	line = parse_texture(m, line, (t_shape *) &cylinder);
	m->cylinders[m->cylinders_length++] = cylinder;
}
