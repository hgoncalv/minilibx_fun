/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_parsing_resolution.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 11:06:09 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/07/25 12:14:13 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	store_resolution(t_game *game, char *line)
{
	int	i;

	i = 0;
	if (game->info.win_width || game->info.win_height)
		free_game(game, 1, "Resolution info doubled");
	find_resolution(game, line, &i);
	if (!game->info.win_width || !game->info.win_height)
		free_game(game, 1, "One of resolutions is invalid");
	while (ft_space(line[i]))
		i++;
	if (line[i] != '\0')
		free_game(game, 1, "Resolution info not corrected");
	return (RESOLUTION);
}

void	find_resolution(t_game *game, char *line, int *i)
{
	while (ft_uppercase(line[*i]))
		(*i)++;
	while (ft_space(line[*i]))
		(*i)++;
	while (ft_digit(line[*i]))
		game->info.win_width = game->info.win_width * 10 + line[(*i)++] - 48;
	while (ft_space(line[*i]))
		(*i)++;
	if (ft_digit(line[*i]))
	{
		while (ft_digit(line[*i]))
			game->info.win_height = game->info.win_height * 10 + line[(*i)++]
				- 48;
	}
	else
		free_game(game, 1, "Resolution info not corrected");
}
