#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include "../minilibx-linux/mlx.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FOV_ANGLE 60.0
#define PLAYER_SPEED 5.0
#define ROTATION_SPEED 3.0
#define KEY_ESC 0xFF1B // ESC key
#define KEY_LEFT 0xFF51 // Left arrow key
#define KEY_RIGHT 0xFF53 // Right arrow key
#define KEY_W 0x0077    // W key
#define KEY_A 0x0061    // A key
#define KEY_S 0x0073    // S key
#define KEY_D 0x0064    // D key

typedef struct s_player {
    double x;
    double y;
    double angle;
} t_player;

typedef struct s_vars {
    void *mlx;
    void *win;
    t_player player;
    struct timeval prev_frame_time;
    int w_key_pressed;
    int a_key_pressed;
    int s_key_pressed;
    int d_key_pressed;
    bool need_update;
} t_vars;


#define MAP_WIDTH 10
#define MAP_HEIGHT 10

int map[MAP_WIDTH][MAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

double normalize_angle(double angle);
double distance_between_points(double x1, double y1, double x2, double y2);
int update_frame(t_vars *vars);

void mlx_line_put(void *mlx_ptr, void *win_ptr, int x1, int y1, int x2, int y2, int color, int thickness)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    for (int i = -thickness / 2; i <= thickness / 2; i++)
    {
        mlx_pixel_put(mlx_ptr, win_ptr, x1, y1 + i, color);
    }

    while (x1 != x2 || y1 != y2)
    {
        int err2 = 2 * err;

        if (err2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if (err2 < dx)
        {
            err += dx;
            y1 += sy;
        }

        for (int i = -thickness / 2; i <= thickness / 2; i++)
        {
            mlx_pixel_put(mlx_ptr, win_ptr, x1, y1 + i, color);
        }
    }
}

void render_map(t_vars *vars)
{
    int cell_width = WINDOW_WIDTH / MAP_WIDTH;
    int cell_height = WINDOW_HEIGHT / MAP_HEIGHT;

    void *image = mlx_new_image(vars->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    unsigned int *image_data = (unsigned int *)mlx_get_data_addr(image, &(int){0}, &(int){0}, &(int){0});

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            int cell_type = map[x][y];
            int cell_x = x * cell_width;
            int cell_y = y * cell_height;

            unsigned int color = cell_type == 1 ? 0x000000 : 0xFFFFFF;
            for (int i = 0;i < 2; i++) //i < cell_height; i++)
            {
                for (int j = 0; j < cell_width; j++)
                {
                    int pixel_x = cell_x + j;
                    int pixel_y = cell_y + i;
                    image_data[pixel_y * WINDOW_WIDTH + pixel_x] = color;
                }
            }
        }
    }

    mlx_put_image_to_window(vars->mlx, vars->win, image, 0, 0);
    mlx_destroy_image(vars->mlx, image);
}

// void render_map(t_vars *vars)
// {

//     // int cell_width = WINDOW_WIDTH / MAP_WIDTH;
//     // int cell_height = WINDOW_HEIGHT / MAP_HEIGHT;

//     // for (int y = 0; y < MAP_HEIGHT; y++)
//     // {
//     //     for (int x = 0; x < MAP_WIDTH; x++)
//     //     {
//     //         int cell_type = map[x][y];
//     //         int cell_x = x * cell_width;
//     //         int cell_y = y * cell_height;

//     //         if (cell_type == 1)
//     //         {
//     //             // Draw a wall
//     //             mlx_pixel_put(vars->mlx, vars->win, cell_x, cell_y, 0x000000);
//     //         }
//     //         else
//     //         {
//     //             // Draw an empty space
//     //             mlx_pixel_put(vars->mlx, vars->win, cell_x, cell_y, 0xFFFFFF);
//     //         }
//     //     }
//     // }

    
// }

// void render_player(t_vars *vars)
// {
//     int player_x = (int)vars->player.x;
//     int player_y = (int)vars->player.y;
//     double angle = vars->player.angle;
//     int arrow_length = 30;
//     int arrow_thickness = 10;

//     // Calculate the coordinates of the arrow's endpoint
//     int arrow_tip_x = player_x + (int)(cos(angle * M_PI / 180.0) * arrow_length);
//     int arrow_tip_y = player_y + (int)(sin(angle * M_PI / 180.0) * arrow_length);

//     // Calculate the perpendicular vector to the arrow
//     double perpendicular_angle = angle - 90.0;
//     int dx = (int)(cos(perpendicular_angle * M_PI / 180.0));
//     int dy = (int)(sin(perpendicular_angle * M_PI / 180.0));

//     // Draw the arrow body
//     for (int i = -arrow_thickness; i <= arrow_thickness; i++)
//     {
//         int body_x = player_x + i * dx;
//         int body_y = player_y + i * dy;
//         mlx_pixel_put(vars->mlx, vars->win, body_x, body_y, 0xFF0000);
//         mlx_pixel_put(vars->mlx, vars->win, arrow_tip_x + i * dx, arrow_tip_y + i * dy, 0xFF0000);
//     }

//     // Draw the arrow head
//     int head_length = arrow_length / 2;
//     for (int i = 0; i <= head_length; i++)
//     {
//         int head_x = arrow_tip_x - (int)(cos(angle * M_PI / 180.0) * i);
//         int head_y = arrow_tip_y - (int)(sin(angle * M_PI / 180.0) * i);
//         mlx_pixel_put(vars->mlx, vars->win, head_x, head_y, 0xFF0000);
//     }
// }

double normalize_angle(double angle)
{
    while (angle < 0.0)
        angle += 360.0;

    while (angle >= 360.0)
        angle -= 360.0;

    return angle;
}

void render_rays(t_vars *vars)
{
    int player_x = (int)vars->player.x;
    int player_y = (int)vars->player.y;
    double player_angle = vars->player.angle;
    double ray_angle;
    int ray_length = 300;
    int ray_thickness = 3;

    for (int i = 1; i >= -1; i--)
    {
        ray_angle = normalize_angle(player_angle + FOV_ANGLE / 2 * i);
        double ray_angle_rad = ray_angle * M_PI / 180.0;

        double ray_x = player_x + ray_length * cos(ray_angle_rad);
        double ray_y = player_y + ray_length * sin(ray_angle_rad);

        // Draw the ray as a line segment
        mlx_line_put(vars->mlx, vars->win, player_x, player_y, ray_x, ray_y, 0x00FF00, ray_thickness);
    }
}




void render_game(t_vars *vars)
{
    // Clear the screen
    mlx_clear_window(vars->mlx, vars->win);

    // Render the map
    // render_map(vars);

    // Draw the player
    render_rays(vars);

    // Reset the need_update flag since we have rendered everything
    vars->need_update = false;

    mlx_do_sync(vars->mlx);
}



void handle_input(t_vars *vars)
{
    // Handle player movement
    if (vars->w_key_pressed)
    {
        vars->player.x += cos(vars->player.angle * M_PI / 180.0) * PLAYER_SPEED;
        vars->player.y += sin(vars->player.angle * M_PI / 180.0) * PLAYER_SPEED;
        vars->need_update = true;
    }
    if (vars->s_key_pressed)
    {
        vars->player.x -= cos(vars->player.angle * M_PI / 180.0) * PLAYER_SPEED;
        vars->player.y -= sin(vars->player.angle * M_PI / 180.0) * PLAYER_SPEED;
        vars->need_update = true;
    }

    // Handle player rotation
    if (vars->a_key_pressed)
    {
        vars->player.angle -= ROTATION_SPEED;
        if (vars->player.angle < 0)
            vars->player.angle += 360.0;
        vars->need_update = true;
    }
    if (vars->d_key_pressed)
    {
        vars->player.angle += ROTATION_SPEED;
        if (vars->player.angle >= 360.0)
            vars->player.angle -= 360.0;
        vars->need_update = true;
    }
    
    
}

int handle_key_press(int keycode, t_vars *vars)
{
    if (keycode == KEY_ESC)
    {
        // Close the window and quit the program
        mlx_destroy_window(vars->mlx, vars->win);
        mlx_destroy_display(vars->mlx);
        free(vars->mlx);
        exit(0);
    }
    else if (keycode == KEY_W)
    {
        vars->w_key_pressed = 1;
    }
    else if (keycode == KEY_A)
    {
        vars->a_key_pressed = 1;
    }
    else if (keycode == KEY_S)
    {
        vars->s_key_pressed = 1;
    }
    else if (keycode == KEY_D)
    {
        vars->d_key_pressed = 1;
    }
    update_frame(vars);
    printf("(x,y,angle) = (%f, %f, %f)\n", vars->player.x, vars->player.y, vars->player.angle);
    // render_game(vars);
    
    return 0;
}

int handle_key_release(int keycode, t_vars *vars)
{
    if (keycode == KEY_W)
    {
        vars->w_key_pressed = 0;
    }
    else if (keycode == KEY_A)
    {
        vars->a_key_pressed = 0;
    }
    else if (keycode == KEY_S)
    {
        vars->s_key_pressed = 0;
    }
    else if (keycode == KEY_D)
    {
        vars->d_key_pressed = 0;
    }

    return 0;
}

int update_frame(t_vars *vars)
{
    // Update game logic
    handle_input(vars);

    // Render the game screen only if there were changes
    if (vars->need_update)
    {
        render_game(vars);
        vars->need_update = false;
    }

    return 0;
}

int main(void)
{
    t_vars vars;

    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Game");

    vars.player.x = WINDOW_WIDTH / 2;
    vars.player.y = WINDOW_HEIGHT / 2;
    vars.player.angle = 0.0;

    vars.w_key_pressed = 0;
    vars.a_key_pressed = 0;
    vars.s_key_pressed = 0;
    vars.d_key_pressed = 0;

    vars.need_update=true;
    update_frame(&vars);
    // render_game(&vars); // Render the initial game screen

    mlx_hook(vars.win, 2, 1L << 0, handle_key_press, &vars);
    mlx_hook(vars.win, 3, 1L << 1, handle_key_release, &vars);

    // Register the update_frame function as the loop hook
    mlx_loop_hook(vars.mlx, update_frame, &vars);

    // Start the main loop
    mlx_loop(vars.mlx);

    return 0;
}