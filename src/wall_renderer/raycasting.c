/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sam <sam@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 11:16:34 by sle-huec          #+#    #+#             */
/*   Updated: 2023/01/19 18:06:00 by sam              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>


void init_dda(t_cub3d *s)
{
	if (s->ray_dir_x < 0)
	{
		s->step_x = -1;
		s->first_dist_x = (s->x - s->tile_x) * s->delta_x;
	}
	else
	{
		s->step_x = 1;
		s->first_dist_x = (s->tile_x + 1 - s->x) * s->delta_x;
	}
	if (s->ray_dir_y < 0)
	{
		s->step_y = -1;
		s->first_dist_y = (s->y - s->tile_y) * s->first_dist_y;
	}
	else
	{
		s->step_y = 1;
		s->first_dist_y = (s->tile_y + 1 - s->y) * s->first_dist_y;
	}
}

void	calculate_data(t_cub3d *s)
{
	s->ratio_camera_x = 2 * s->col_x_iterator / (double)WIDTH - 1;
	s->ray_dir_x = s->v_dir_x + s->v_camera_plane_x * s->ratio_camera_x;
	s->ray_dir_y = s->v_dir_y + s->v_camera_plane_y * s->ratio_camera_x;
	s->tile_x = (int)s->x;
	s->tile_y = (int)s->y;
	s->delta_x = fabs(1 / s->ray_dir_x);
	s->delta_y = fabs(1 / s->ray_dir_y);
	init_dda(s);
}

void	dda(t_cub3d *s)
{
	if (s->first_dist_x < s->first_dist_y)
	{
		s->first_dist_x += s->delta_x;
		s->tile_x += s->step_x;
		s->side = 0;
	}
	else
	{
		s->first_dist_y += s->delta_y;
		s->tile_y += s->step_y;
		s->side = 1;
	}
}

void	calculate_dist_on_camera(t_cub3d *s)
{
	if (s->side == 0)
		s->perp_wall_dist = (s->first_dist_x - s->delta_x);
	else
		s->perp_wall_dist = (s->first_dist_y - s->delta_y);
}

// si ca plante rajouter 1e30 ds if else voir tuto
void	cast_ray(t_cub3d *s)
{
	int		pos_tile_x;
	int		pos_tile_y;
	t_img	img;

	pos_tile_x = floor(s->tile_x);
	pos_tile_y = floor(s->tile_y);


	// print_raycasting_data(s);
	img.img_str = mlx_get_data_addr(s->win_buffer,
		&img.bits, &img.line, &img.endian);
	s->col_x_iterator = 0;
	while (s->col_x_iterator < WIDTH)
	{
		calculate_data(s);
		s-> flag_hit_wall = 0;
		while (s->flag_hit_wall == 0)
		{
			dda(s);
			calculate_dist_on_camera(s);
			if (s->map[pos_tile_x][pos_tile_y] > 0)
				s->flag_hit_wall = 1;
		}
		draw(s, &img);
		s->col_x_iterator++;
	}
}
