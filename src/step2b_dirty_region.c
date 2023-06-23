#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include "../minilibx-linux/mlx.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define RECTANGLE_WIDTH 200
#define RECTANGLE_HEIGHT 150
#define FRAME_TIME_MICROSECONDS 16667 // 60 fps in microseconds
#define KEY_W 0x0077  // W key
#define KEY_A 0x0061  // A key
#define KEY_S 0x0073  // S key
#define KEY_D 0x0064  // D key

typedef struct s_vars {
    void *mlx;
    void *win;
    void *img;
    char *addr;
    int rectangle_x;
    int rectangle_y;
    int prev_rectangle_x;
    int prev_rectangle_y;
    struct timeval prev_frame_time;
    int first_frame_rendered;
    int w_key_pressed;
    int a_key_pressed;
    int s_key_pressed;
    int d_key_pressed;
} t_vars;

void ft_memset(void *ptr, int value, size_t num)
{
    unsigned char *byte_ptr = (unsigned char *)ptr;
    size_t i;
    
    for (i = 0; i < num; i++)
    {
        *byte_ptr = (unsigned char)value;
        byte_ptr++;
    }
}

void mlx_pixel_put_img(void *img, int x, int y, int color);

void draw_rectangle(char *addr, int x, int y, int width, int height)
{
    int i, j;

    i = x;
    while (i < x + width)
    {
        j = y;
        while (j < y + height)
        {
            mlx_pixel_put_img(addr, i, j, 0xFFFFFF);
            j++;
        }
        i++;
    }
}

void mlx_pixel_put_img(void *img, int x, int y, int color)
{
    char *dst;

    dst = img + (y * WINDOW_WIDTH * 4) + (x * 4);
    *(unsigned int *)dst = color;
}

int check_set_time_delay(t_vars *vars)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    long elapsed_time = (current_time.tv_sec - vars->prev_frame_time.tv_sec) * 1000000 +
                        (current_time.tv_usec - vars->prev_frame_time.tv_usec);

    long frame_time = FRAME_TIME_MICROSECONDS;

    if (elapsed_time < frame_time)
    {
        usleep(frame_time - elapsed_time);
        return 0; // No frame update
    }

    vars->prev_frame_time = current_time;
    return 1; // Frame update
}

void update_rectangle_position(t_vars *vars, int x, int y)
{
    vars->rectangle_x += x;
    vars->rectangle_y += y;

    // Ensure the rectangle stays within the window bounds
    if (vars->rectangle_x < 0)
        vars->rectangle_x = 0;
    else if (vars->rectangle_x + RECTANGLE_WIDTH >= WINDOW_WIDTH)
        vars->rectangle_x = WINDOW_WIDTH - RECTANGLE_WIDTH - 1;

    if (vars->rectangle_y < 0)
        vars->rectangle_y = 0;
    else if (vars->rectangle_y + RECTANGLE_HEIGHT >= WINDOW_HEIGHT)
        vars->rectangle_y = WINDOW_HEIGHT - RECTANGLE_HEIGHT - 1;
}

void initialize_vars(t_vars *vars)
{
    vars->mlx = mlx_init();
    vars->win = mlx_new_window(vars->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Wolfenstein Replica - Step 3b");

    vars->img = mlx_new_image(vars->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    vars->addr = mlx_get_data_addr(vars->img, &(int){0}, &(int){0}, &(int){0});

    // Set the initial position for the rectangle
    vars->rectangle_x = (WINDOW_WIDTH - RECTANGLE_WIDTH) / 2;
    vars->rectangle_y = (WINDOW_HEIGHT - RECTANGLE_HEIGHT) / 2;

    // Initialize the previous rectangle position to the initial position
    vars->prev_rectangle_x = vars->rectangle_x;
    vars->prev_rectangle_y = vars->rectangle_y;

    // Get the current time for the first frame
    gettimeofday(&(vars->prev_frame_time), NULL);

    // Initialize the first frame rendered flag to 0
    vars->first_frame_rendered = 0;

    // Initialize key states
    vars->w_key_pressed = 0;
    vars->a_key_pressed = 0;
    vars->s_key_pressed = 0;
    vars->d_key_pressed = 0;
}

int update_frame(void *param)
{
    t_vars *vars = (t_vars *)param;

    // Check and set the time delay
    if (!check_set_time_delay(vars))
        return 0;

    // Only redraw the rectangle if its position has changed or it's the first frame
    if (vars->rectangle_x != vars->prev_rectangle_x || vars->rectangle_y != vars->prev_rectangle_y || !vars->first_frame_rendered)
    {
        // Clear the buffer
        ft_memset(vars->addr, 0, WINDOW_WIDTH * WINDOW_HEIGHT * 4);

        // Draw the rectangle on the buffer
        draw_rectangle(vars->addr, vars->rectangle_x, vars->rectangle_y, RECTANGLE_WIDTH, RECTANGLE_HEIGHT);

        // Copy the buffer to the window
        mlx_put_image_to_window(vars->mlx, vars->win, vars->img, 0, 0);

        // Update the previous rectangle position
        vars->prev_rectangle_x = vars->rectangle_x;
        vars->prev_rectangle_y = vars->rectangle_y;

        // Set the first frame rendered flag to 1
        vars->first_frame_rendered = 1;
    }

    return 0;
}

int handle_key_press(int keycode, t_vars *vars)
{
    // Set the corresponding key state to pressed
    if (keycode == KEY_W)
        vars->w_key_pressed = 1;
    else if (keycode == KEY_A)
        vars->a_key_pressed = 1;
    else if (keycode == KEY_S)
        vars->s_key_pressed = 1;
    else if (keycode == KEY_D)
        vars->d_key_pressed = 1;
    else
        return 0; // No frame update

    // Move the rectangle based on the combination of pressed keys
    int move_x = 0;
    int move_y = 0;

    if (vars->w_key_pressed && !vars->s_key_pressed)
        move_y -= 10;
    else if (!vars->w_key_pressed && vars->s_key_pressed)
        move_y += 10;

    if (vars->a_key_pressed && !vars->d_key_pressed)
        move_x -= 10;
    else if (!vars->a_key_pressed && vars->d_key_pressed)
        move_x += 10;

    if (move_x != 0 || move_y != 0)
        update_rectangle_position(vars, move_x, move_y);
    else
        return 0; // No frame update

    return 1; // Frame update
}

int handle_key_release(int keycode, t_vars *vars)
{
    // Set the corresponding key state to released
    if (keycode == KEY_W)
        vars->w_key_pressed = 0;
    else if (keycode == KEY_A)
        vars->a_key_pressed = 0;
    else if (keycode == KEY_S)
        vars->s_key_pressed = 0;
    else if (keycode == KEY_D)
        vars->d_key_pressed = 0;
    else
        return 0; // No frame update

    return 1; // Frame update
}

int handle_close_event(t_vars *vars)
{
    mlx_loop_end(vars->mlx); // Terminate the event loop
    return 0;
}

void cleanup_vars(t_vars *vars)
{
    mlx_destroy_image(vars->mlx, vars->img);
    mlx_destroy_window(vars->mlx, vars->win);
    mlx_destroy_display(vars->mlx);
    free(vars->mlx);
}

int main(void)
{
    t_vars vars;

    // Initialize all members of t_vars structure
    ft_memset(&vars, 0, sizeof(t_vars));

    initialize_vars(&vars);

    // Event loop to continuously update the window
    mlx_loop_hook(vars.mlx, update_frame, &vars);

    // Keyboard input handling
    mlx_hook(vars.win, 2, 1L << 0, handle_key_press, &vars);
    mlx_hook(vars.win, 3, 1L << 1, handle_key_release, &vars);

    // Close event handling
    mlx_hook(vars.win, 17, 1L << 0, handle_close_event, &vars);

    // Event loop to keep the window open
    mlx_loop(vars.mlx);

    cleanup_vars(&vars);

    return 0;
}
