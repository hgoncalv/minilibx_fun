// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   exit_free.c                                        :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/02/16 21:26:04 by mgranate_ls       #+#    #+#             */
// /*   Updated: 2023/07/21 12:34:07 by hgoncalv         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../cub3d.h"

// int	close_window(t_cub *vars)
// {
// 	exit_free(vars, 0, "Exit Program Succesfully");
// 	return (0);
// }

// void	free_imgs(t_cub *data)
// {
// 	int	i;

// 	i = 0;
// 	while (i < 8)
// 	{
// 		if (data->img.wall[i].ptr)
// 			mlx_destroy_image(data->mlx, data->img.wall[i].ptr);
// 		data->img.wall[i].ptr = 0;
// 		i++;
// 	}
// 	i = 0;
// 	while (i < 2)
// 	{
// 		if (data->sprites[i].ptr)
// 			mlx_destroy_image(data->mlx, data->sprites[i].ptr);
// 		data->sprites[i].ptr = 0;
// 		i++;
// 	}
// 	if (data->screen.ptr)
// 		mlx_destroy_image(data->mlx, data->screen.ptr);
// 	data->screen.ptr = 0;
// }

void	free_mlx(t_game	*game)
{
	// free_imgs(data);
	if (game->win != NULL)
	{
		mlx_destroy_display(game->mlx);
		// mlx_destroy_window(game->mlx, game->win);
		free(game->mlx);
		game->mlx = NULL;
		game->win = NULL;
	}
	else if (game->mlx != NULL)
	{
		free(game->mlx);
		game->mlx = NULL;
	}
    
    // if(game->map.tmp)
    //     {free(game->map.tmp);}
	
}

// void	exit_prs(t_prs *data, int status, char *str)
// {
// 	if (data)
// 	{
// 		if (data->file)
// 			alloc().free_matrix((void **)data->file);
// 		if (data->floor_c)
// 			alloc().free_array((void *)data->floor_c);
// 		if (data->color_c)
// 			alloc().free_array((void *)data->color_c);
// 	}
// 	exit_free(cube(), status, str);
// }

void	exit_free(t_game *game, int status, char *str)
{
	// int	i;

	// i = -1;
	if (status)
		printf("Error: %s\n", str);
	else
		printf("%s\n", str);
	// if(game->map.buff)
    //     free_tab2(game->map.buff);
    if (game->map.map)
	{
		free_tab2(game->map.map);
		game->map.map = NULL;
	}
	// if (game->img.path)
	// {
	// 	while (++i < 4)
	// 		if (game->img.path[i])
	// 			alloc().free_array((void *) game->img.path[i]);
	// 	free(game->img.path);
	// }
	free_mlx(game);
	exit(status);
}

void free_game(t_game *game) {
    if (game == NULL) return;

    // Free t_info members
    for (int i = 0; i < TEXTURE; i++) {
        // free(game->info.tex_path[i]);
        // free(game->info.texture[i]);
    }

    // Free t_map members
    for (int i = 0; i < game->map.height_count; i++) {
        free(game->map.map[i]);
    }
     for (int i = 0; i < game->info.win_height; i++) {
        free(game->buf[i]);
    }
    free(game->buf);
    free(game->map.map);
    free(game->info.line);
    // free(game->buf);
    // free(game->map.tmp);
    // free(game->map.tmp);
    // if(game->map.buff != NULL)
        // free_tab2(game->map.buff);

    // Free t_img member
    // free(game->img.data);
    mlx_destroy_image(game->mlx, game->img.img_ptr);
    // Note: No need to free img.img_ptr as it's managed by the graphics library.

    // Free other dynamically allocated arrays in t_game
    // free_tab2(game->buf);
    free(game->z_buffer);

    // Finally, free the t_game structure itself
    // free(game);
    // Destroy the MiniLibX window and display
    if (game->mlx && game->win) {
        mlx_destroy_window(game->mlx, game->win);
    }
    if (game->mlx) {
        mlx_destroy_display(game->mlx);
        free(game->mlx);
    }
    error_msg("Dodge THIS !!!");
}