#include <stdio.h>
#include "../minilibx-linux/mlx.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define RECTANGLE_WIDTH 200
#define RECTANGLE_HEIGHT 150

typedef struct s_vars {
    void *mlx;
    void *win;
} t_vars;

void draw_rectangle(void *mlx, void *win, int x, int y, int width, int height)
{
    int i;
    int j;

    i = x;
    while (i < x + width)
    {
        j = y;
        while (j < y + height)
        {
            mlx_pixel_put(mlx, win, i, j, 0xFFFFFF);
            j++;
        }
        i++;
    }
}

int draw_frame(void *param)
{
    t_vars *vars = (t_vars *)param;

    // Clear the window
    mlx_clear_window(vars->mlx, vars->win);

    // Set the initial position for the rectangle
    int rectangle_x = (WINDOW_WIDTH - RECTANGLE_WIDTH) / 2;
    int rectangle_y = (WINDOW_HEIGHT - RECTANGLE_HEIGHT) / 2;

    // Draw the rectangle
    draw_rectangle(vars->mlx, vars->win, rectangle_x, rectangle_y, RECTANGLE_WIDTH, RECTANGLE_HEIGHT);

    return 0;
}

int main(void)
{
    t_vars vars;

    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Wolfenstein Replica - Step 3");

    // Event loop to continuously update the window
    // mlx_loop_hook(vars.mlx, draw_frame, &vars);
    draw_frame(&vars);
    // Event loop to keep the window open
    mlx_loop(vars.mlx);

    return 0;
}
