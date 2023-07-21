/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_player_move.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 11:05:16 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/07/21 11:06:37 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	move_forward(t_game *game, t_player *player)
{
	char	x;
	char	y;

	x = game->map.map[(int)player->y]
	[(int)(player->x + player->dir_x * player->move_speed)];
	y = game->map.map[(int)(player->y + player->dir_y * player->move_speed)]
	[(int)player->x];
	if (x != '1')
		player->x += player->dir_x * player->move_speed;
	if (y != '1')
		player->y += player->dir_y * player->move_speed;
}

void	move_backward(t_game *game, t_player *player)
{
	char	x;
	char	y;

	x = game->map.map[(int)player->y]
	[(int)(player->x - player->dir_x * player->move_speed)];
	y = game->map.map[(int)(player->y - player->dir_y * player->move_speed)]
	[(int)player->x];
	if (x != '1')
		player->x -= player->dir_x * player->move_speed;
	if (y != '1')
		player->y -= player->dir_y * player->move_speed;
}

void	move_right(t_game *game, t_player *player)
{
	char	x;
	char	y;

	x = game->map.map[(int)player->y]
	[(int)(player->x + player->dir_y * player->move_speed)];
	y = game->map.map[(int)(player->y + -player->dir_x * player->move_speed)]
	[(int)player->x];
	if (x != '1')
		player->x += player->dir_y * player->move_speed;
	if (y != '1')
		player->y += -player->dir_x * player->move_speed;
}

void	move_left(t_game *game, t_player *player)
{
	char	x;
	char	y;

	x = game->map.map[(int)player->y]
	[(int)(player->x + player->dir_y * -player->move_speed)];
	y = game->map.map[(int)(player->y + -player->dir_x * -player->move_speed)]
	[(int)player->x];
	if (x != '1')
		player->x += player->dir_y * -player->move_speed;
	if (y != '1')
		player->y += -player->dir_x * -player->move_speed;
}
