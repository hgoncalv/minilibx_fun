/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 11:32:22 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/07/25 11:58:04 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	free_game_p1(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (game->info.tex_path[i] != NULL)
			free(game->info.tex_path[i]);
		i++;
	}
	if (game->map.tmp != NULL)
		free(game->map.tmp);
	if (game->map.buff != NULL)
		free_tab2(game->map.buff);
	i = 0;
	if (game->map.map != NULL)
	{
		while (i < game->map.height_count)
			free(game->map.map[i++]);
		free(game->map.map);
	}
}

void	free_game_p2(t_game *game)
{
	int	i;

	i = 0;
	if (game->buf != NULL)
	{
		while (i < game->info.win_height)
			free(game->buf[i++]);
		free(game->buf);
	}
	if (game->info.line)
		free(game->info.line);
	if (game->leftover_str != NULL)
		free(game->leftover_str);
	if (game->img.img_ptr)
		mlx_destroy_image(game->mlx, game->img.img_ptr);
	if (game->z_buffer)
		free(game->z_buffer);
}

void	free_game(t_game *game, int status, char *str)
{
	if (game == NULL)
		return ;
	free_game_p1(game);
	free_game_p2(game);
	if (game->mlx && game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	if (status)
		error_msg(str);
	ft_putstr(str);
	ft_putchar('\n');
	exit(0);
}
