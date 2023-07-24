/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_parsing_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 11:06:12 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/07/21 17:06:05 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	store_color(int info, t_game *game, char *line)
{
	int		i;
	int		color;
	char	**rgb;
	char	*ret_str;
	i = 0;
	color = 0;
	ret_str = 0;
	ret_str = check_color_line(game, line, &info, &i);
	if(ret_str)
	{
		free(line);
		free_game(game , 1, ret_str);
	}
	rgb = ft_split(&line[i], ',');
	if (!rgb)
		free_game(game , 1, "ERROR: RGB Split error");
	i = 0;
	ret_str = find_color(line, &i, rgb, &color);
	if(ret_str)
	{
		free_tab2(rgb);
		free(line);
		free_game(game , 1, ret_str);
	}
	if (info == FLOOR)
		game->info.floor_color = color;
	else if (info == CEILING)
		game->info.ceiling_color = color;
	free_tab2(rgb);
	return (info);
}

char	*check_color_line(t_game *game, char *line, int *info, int *i)
{
	if ((game->info.mark == *info && game->info.floor_color != -1)
		|| (game->info.mark == *info && game->info.ceiling_color != -1))
		return("Color info doubled");
	while (ft_uppercase(line[*i]))
		(*i)++;
	while (ft_space(line[*i]))
		(*i)++;
	return (0);
}

char	*find_color(char *line, int *i, char **rgb, int *color)
{
	int	tmp;
	int	j;

	j = *i;
	while (*i < 3)
	{
		if (!rgb[*i] || !ft_only_digit(rgb[*i]))
		{
			printf("aqui");
			return("RGB info not correct");
		}
		tmp = ft_atoi(rgb[*i]);
		if (tmp > 255 || tmp < 0)
			return("RGB range invalid");
		*color = *color * 256 + tmp;
		(*i)++;
	}
	while (line[j] != '\0')
	{
		if (line[j] == ',' && line[j + 1] == '\0')
			return("Unexpected comma exist");
		j++;
	}
	if (rgb[3])
		return("Too many color for RGB");
	return (0);
}
