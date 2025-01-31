/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:03:43 by lfiestas          #+#    #+#             */
/*   Updated: 2025/01/31 17:23:58 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "get_next_line.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>

// TODO with normalized vectors, check if their value equal to 1.0. If not,
// normalize with warning.

double	str_to_f(const char *str)
{
	uint64_t	integer;
	uint64_t	fraction;
	size_t		fraction_power;
	double		sign;

	sign = 1.;
	if ((*str == '+' || *str == '-'))
		sign = 1. - 2 * (*str++ == '-');
	integer = 0;
	while ('0' <= *str && *str <= '9' && integer < UINT64_MAX / 10 - 10)
		integer = integer * 10 + *str++ - '0';
	if (*str == '.')
		++str;
	fraction = 0;
	fraction_power = 1;
	while ('0' <= *str && *str <= '9' && fraction < UINT64_MAX / 10 - 10)
	{
		fraction_power *= 10;
		fraction = fraction * 10 + *str++ - '0';
	}
	if (integer >= UINT64_MAX / 10 - 10 || fraction >= UINT64_MAX / 10 - 10)
		return (sign * INFINITY);
	return (sign * (integer + (double)fraction / fraction_power));
}

static bool	parse_ambient_light(t_minirt *mrt, const char *line)
{
	while (ft_isspace(*line))
		++line;

}

static void	parse_line(t_minirt *mrt, bool found[128], const char *line)
{
	while (ft_isspace(*line))
		++line;
	if (line[0] == 'A' && ft_isspace(line[1])
		&& mrt_assert(mrt, !found['A'], "Multiple ambient lights"))
		found['A'] = parse_ambient_light(mrt, line + ft_strlen("A "));
	else if (line[0] == 'C' && ft_isspace(line[1])
		&& mrt_assert(mrt, !found['C'], "Multiple cameras"))
		found['C'] = parse_camera(mrt, line + ft_strlen("C "));
	else if (line[0] == 'L' && ft_isspace(line[1])
		&& mrt_assert(mrt, !found['L'], "Multiple lights"))
		found['L'] = parse_ligth(mrt, line + ft_strlen("L "));
	else if (line[0] == 's' && line[1] == 'p' && ft_isspace(line[2]))
		parse_sphere(mrt, line + ft_strlen("sp "));
	else if (line[0] == 'p' && line[1] == 'l' && ft_isspace(line[2]))
		parse_plane(mrt, line + ft_strlen("pl "));
	else if (line[0] == 'c' && line[1] == 'y' && ft_isspace(line[2]))
		parse_cylinder(mrt, line + ft_strlen("cy "));
	mrt_assert(mrt, false, "Invalid line in input file");
}

void	parse_input(t_minirt *mrt, const char *path)
{
	int			fd;
	static bool	found[128];

	fd = open(path, O_RDONLY);
	mrt_assert(mrt, fd != -1, path);
	while (true)
	{
		mrt->line = get_next_line(fd);
		if (line == NULL)
			break;
		while (ft_isspace(mrt->line[ft_strlen(mrt->line) - 1]))
			mrt->line[ft_strlen(line) - 1] = '\0';
		if (mrt->line[0] != '\0')
			parse_line(mrt, found, mrt->line);
		free(mrt->line);
	}
	mrt_assert(mrt, found['A'], "No ambient light specified");
	mrt_assert(mrt, found['C'], "No camera specified");
	mrt_assert(mrt, found['L'], "No light specified");
}
