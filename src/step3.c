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
} t_vars;

void handle_input(t_vars *vars)
{
    // Handle player movement
    if (vars->w_key_pressed)
    {
        vars->player.x += cos(vars->player.angle * M_PI / 180.0) * PLAYER_SPEED;
        vars->player.y += sin(vars->player.angle * M_PI / 180.0) * PLAYER_SPEED;
    }
    if (vars->s_key_pressed)
    {
        vars->player.x -= cos(vars->player.angle * M_PI / 180.0) * PLAYER_SPEED;
        vars->player.y -= sin(vars->player.angle * M_PI / 180.0) * PLAYER_SPEED;
    }

    // Handle player rotation
    if (vars->a_key_pressed)
    {
        vars->player.angle -= ROTATION_SPEED;
        if (vars->player.angle < 0)
            vars->player.angle += 360.0;
    }
    if (vars->d_key_pressed)
    {
        vars->player.angle += ROTATION_SPEED;
        if (vars->player.angle >= 360.0)
            vars->player.angle -= 360.0;
    }
    printf("(x,y,angle) = (%f, %f, %f)\n", vars->player.x, vars->player.y, vars->player.angle);
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

    mlx_hook(vars.win, 2, 1L << 0, handle_key_press, &vars);
    mlx_hook(vars.win, 3, 1L << 1, handle_key_release, &vars);

    while (1)
    {
        handle_input(&vars);

        // Clear the screen
        mlx_clear_window(vars.mlx, vars.win);

        // Draw the player
        mlx_pixel_put(vars.mlx, vars.win, vars.player.x, vars.player.y, 0xFF0000);
        mlx_do_sync(vars.mlx);
        mlx_loop(vars.mlx);
    }

    return 0;
}
