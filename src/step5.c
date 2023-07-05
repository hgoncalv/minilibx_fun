
#include "../minilibx-linux/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

#define screenWidth 640
#define screenHeight 480
#define mapWidth 24
#define mapHeight 24

#define KEY_ESC 0xFF1B // ESC key
#define KEY_LEFT 0xFF51 // Left arrow key
#define KEY_RIGHT 0xFF53 // Right arrow key
#define KEY_W 0x0077    // W key
#define KEY_A 0x0061    // A key
#define KEY_S 0x0073    // S key
#define KEY_D 0x0064    // D key

typedef struct s_cast_vars {
    double cameraX;
    double rayDirX;
    double rayDirY;
    // Map position
    int mapX;
    int mapY;
    // Length of ray from current position to next x or y-side
    double sideDistX;
    double sideDistY;
    // Length of ray from one x or y-side to next x or y-side
    double deltaDistX;
    double deltaDistY;
    double perpWallDist;
    // Direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;
    int hit; // Was a wall hit?
    int side;
    int x;
} t_cast_vars;

typedef struct s_vars {
    void *mlx;
    void *mlx_win;
    double posX;
    double posY;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
    t_cast_vars *cast_vars;
    void *buffer_img;
    bool update_render;
} t_vars;



int worldMap[24][24] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void ray_calculations(t_vars *vars)
{
    // Calculate ray position and direction
    vars->cast_vars->cameraX = 2 * vars->cast_vars->x / (double)screenWidth - 1;
    vars->cast_vars->rayDirX = vars->dirX + vars->planeX * vars->cast_vars->cameraX;
    vars->cast_vars->rayDirY = vars->dirY + vars->planeY * vars->cast_vars->cameraX;

    // Map position
    vars->cast_vars->mapX = (int)vars->posX;
    vars->cast_vars->mapY = (int)vars->posY;

    // Length of ray from current position to next x or y-side
    
    // Length of ray from one x or y-side to next x or y-side
    vars->cast_vars->deltaDistX = fabs(1 / vars->cast_vars->rayDirX);
    vars->cast_vars->deltaDistY = fabs(1 / vars->cast_vars->rayDirY);
    vars->cast_vars->perpWallDist;
}

void calculate_step_n_inicial_sidedist(t_vars *vars)
{
    if (vars->cast_vars->rayDirX < 0)
    {
        vars->cast_vars->stepX = -1;
        vars->cast_vars->sideDistX = (vars->posX - vars->cast_vars->mapX) * vars->cast_vars->deltaDistX;
    }
    else
    {
        vars->cast_vars->stepX = 1;
        vars->cast_vars->sideDistX = (vars->cast_vars->mapX + 1.0 - vars->posX) * vars->cast_vars->deltaDistX;
    }
    if (vars->cast_vars->rayDirY < 0)
    {
        vars->cast_vars->stepY = -1;
        vars->cast_vars->sideDistY = (vars->posY - vars->cast_vars->mapY) * vars->cast_vars->deltaDistY;
    }
    else
    {
        vars->cast_vars->stepY = 1;
        vars->cast_vars->sideDistY = (vars->cast_vars->mapY + 1.0 - vars->posY) * vars->cast_vars->deltaDistY;
    }
}

void perform_dda(t_vars *vars)
{
    // Perform DDA
    while (vars->cast_vars->hit == 0)
    {
        // Jump to next map square in x or y-direction
        if (vars->cast_vars->sideDistX < vars->cast_vars->sideDistY)
        {
            vars->cast_vars->sideDistX += vars->cast_vars->deltaDistX;
            vars->cast_vars->mapX += vars->cast_vars->stepX;
            vars->cast_vars->side = 0;
        }
        else
        {
            vars->cast_vars->sideDistY += vars->cast_vars->deltaDistY;
            vars->cast_vars->mapY += vars->cast_vars->stepY;
            vars->cast_vars->side = 1;
        }

        // Check if ray has hit a wall
        if (worldMap[vars->cast_vars->mapX][vars->cast_vars->mapY] > 0)
            vars->cast_vars->hit = 1;
    }
}

int performRaycasting(t_vars *vars)
{
    
    // Clear the previous frame
    mlx_clear_window(vars->mlx, vars->mlx_win);

    // Raycasting and rendering logic
    for (vars->cast_vars->x = 0; vars->cast_vars->x < screenWidth; vars->cast_vars->x++)
    {
        ray_calculations(vars);
        // Direction to step in x or y-direction (either +1 or -1)
        vars->cast_vars->hit = 0; // Was a wall hit?
        calculate_step_n_inicial_sidedist(vars);
        // Calculate step and initial sideDist
        perform_dda(vars);   
        // Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
        if (vars->cast_vars->side == 0)
            vars->cast_vars->perpWallDist = (vars->cast_vars->mapX - vars->posX + (1 - vars->cast_vars->stepX) / 2) / vars->cast_vars->rayDirX;
        else
            vars->cast_vars->perpWallDist = (vars->cast_vars->mapY - vars->posY + (1 - vars->cast_vars->stepY) / 2) / vars->cast_vars->rayDirY;
        // Calculate height of line to draw on screen
        int lineHeight = (int)(screenHeight / vars->cast_vars->perpWallDist);
        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight)
            drawEnd = screenHeight - 1;
        // Choose wall color based on the tile type
        int color;
        switch (worldMap[vars->cast_vars->mapX][vars->cast_vars->mapY])
        {
            case 1: // Red
                color = 0xFF0000;
                break;
            case 2: // Green
                color = 0x00FF00;
                break;
            case 3: // Blue
                color = 0x0000FF;
                break;
            default: // Yellow
                color = 0xFFFF00;
                break;
        }
        // Draw the pixels of the stripe as a vertical line
        mlx_pixel_put(vars->mlx, vars->mlx_win, vars->cast_vars->x, drawStart, color);
        mlx_pixel_put(vars->mlx, vars->mlx_win, vars->cast_vars->x, drawEnd, color);
    }
    if(vars->buffer_img)
        mlx_destroy_image(vars->mlx, vars->buffer_img);
    vars->buffer_img = mlx_new_image(vars->mlx, screenWidth, screenHeight);
    mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->buffer_img, 0, 0);
    return (0);
}

int handle_key_press(int keycode, t_vars *vars)
{
    double rotationSpeed = 0.05; // Adjust the rotation speed as needed
    
    if (keycode == KEY_ESC)
    {
        // Close the window and quit the program
        mlx_destroy_window(vars->mlx, vars->mlx_win);
        mlx_destroy_display(vars->mlx);
        free(vars->mlx);
        exit(0);
    }
    else if (keycode == KEY_W)
    {
        // Move forward
        if (worldMap[(int)(vars->posX + vars->dirX)][(int)(vars->posY)] == 0)
            vars->posX += vars->dirX * 0.1;
        if (worldMap[(int)(vars->posX)][(int)(vars->posY + vars->dirY)] == 0)
            vars->posY += vars->dirY * 0.1;
    }
    else if (keycode == KEY_A)
    {
        // Strafe left
        if (worldMap[(int)(vars->posX - vars->dirY)][(int)(vars->posY)] == 0)
            vars->posX -= vars->dirY * 0.1;
        if (worldMap[(int)(vars->posX)][(int)(vars->posY + vars->dirX)] == 0)
            vars->posY += vars->dirX * 0.1;
    }
    else if (keycode == KEY_S)
    {
        // Move backward
        if (worldMap[(int)(vars->posX - vars->dirX)][(int)(vars->posY)] == 0)
            vars->posX -= vars->dirX * 0.1;
        if (worldMap[(int)(vars->posX)][(int)(vars->posY - vars->dirY)] == 0)
            vars->posY -= vars->dirY * 0.1;
    }
    else if (keycode == KEY_D)
    {
        // Strafe right
        if (worldMap[(int)(vars->posX + vars->dirY)][(int)(vars->posY)] == 0)
            vars->posX += vars->dirY * 0.1;
        if (worldMap[(int)(vars->posX)][(int)(vars->posY - vars->dirX)] == 0)
            vars->posY -= vars->dirX * 0.1;
    }
    else if (keycode == KEY_LEFT)
    {
        // Rotate left
        double oldDirX = vars->dirX;
        vars->dirX = vars->dirX * cos(rotationSpeed) - vars->dirY * sin(rotationSpeed);
        vars->dirY = oldDirX * sin(rotationSpeed) + vars->dirY * cos(rotationSpeed);
        double oldPlaneX = vars->planeX;
        vars->planeX = vars->planeX * cos(rotationSpeed) - vars->planeY * sin(rotationSpeed);
        vars->planeY = oldPlaneX * sin(rotationSpeed) + vars->planeY * cos(rotationSpeed);
    }
    else if (keycode == KEY_RIGHT)
    {
        // Rotate right
        double oldDirX = vars->dirX;
        vars->dirX = vars->dirX * cos(-rotationSpeed) - vars->dirY * sin(-rotationSpeed);
        vars->dirY = oldDirX * sin(-rotationSpeed) + vars->dirY * cos(-rotationSpeed);
        double oldPlaneX = vars->planeX;
        vars->planeX = vars->planeX * cos(-rotationSpeed) - vars->planeY * sin(-rotationSpeed);
        vars->planeY = oldPlaneX * sin(-rotationSpeed) + vars->planeY * cos(-rotationSpeed);
    }
    return (0);
}



int main(void)
{
    

    t_vars vars;
    t_cast_vars cast_vars;
    vars.posX = 22;
    vars.posY = 12;
    vars.dirX = -1;
    vars.dirY = 0;
    vars.planeX = 0;
    vars.planeY = 0.66;
    vars.buffer_img = NULL;
    vars.update_render=1;
    vars.cast_vars=&cast_vars;
    vars.mlx = mlx_init();
    vars.mlx_win = mlx_new_window(vars.mlx, screenWidth, screenHeight, "Raycaster");

    
    mlx_hook(vars.mlx_win, 2, 1L << 0, handle_key_press, &vars); 
    mlx_loop_hook(vars.mlx, performRaycasting, &vars);
    mlx_loop(vars.mlx);
    return 0;
}
