#include "../minilibx-linux/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <X11/Xlib.h>

// #define screenWidth 1024
// #define screenHeight 768
#define mapWidth 24
#define mapHeight 24

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define RECTANGLE_WIDTH 200
#define RECTANGLE_HEIGHT 150

#define KEY_ESC 0xFF1B // ESC key
#define KEY_LEFT 0xFF51 // Left arrow key
#define KEY_RIGHT 0xFF53 // Right arrow key
#define KEY_UP 0xFF52    // Up arrow key
#define KEY_DOWN 0xFF54  // Down arrow key
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
    void *win;
    double posX;
    double posY;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
    t_cast_vars *cast_vars;
    void *buffer_img;
    int *buffer_data;
    bool update_render;
    int bits_per_pixel;
    int size_line;
    int endian;
    int screenWidth;
    int screenHeight;
} t_vars;


void draw_rectangle(void *mlx, void *win, int x, int y, int width, int height);
int draw_frame(void *param);


int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void draw_line(int *buffer_data, int x0, int y0, int x1, int y1, int color, int screenWidth)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        // Calculate the index of the pixel in the buffer image data
        int pixel_index = y0 * screenWidth + x0;
        // Set the pixel color directly in the buffer image data
        buffer_data[pixel_index] = color;

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}



void ray_calculations(t_vars *vars)
{
    // Calculate ray position and direction
    vars->cast_vars->cameraX = 2 * vars->cast_vars->x / (double)vars->screenWidth - 1;
    vars->cast_vars->rayDirX = vars->dirX + vars->planeX * vars->cast_vars->cameraX;
    vars->cast_vars->rayDirY = vars->dirY + vars->planeY * vars->cast_vars->cameraX;
    vars->cast_vars->perpWallDist = 0.0;

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

        // break if outside map
        if (vars->cast_vars->mapX < 0 || vars->cast_vars->mapX >= mapWidth || vars->cast_vars->mapY < 0 || vars->cast_vars->mapY >= mapHeight)
            break;
        // Check if ray has hit a wall
        if (worldMap[vars->cast_vars->mapX][vars->cast_vars->mapY] > 0)
            vars->cast_vars->hit = 1;
    }
}

int interpolate_color(int color1, int color2, float weight)
{
    int red1 = (color1 >> 16) & 0xFF;
    int green1 = (color1 >> 8) & 0xFF;
    int blue1 = color1 & 0xFF;

    int red2 = (color2 >> 16) & 0xFF;
    int green2 = (color2 >> 8) & 0xFF;
    int blue2 = color2 & 0xFF;

    int interpolatedRed = (int)(red1 + weight * (red2 - red1));
    int interpolatedGreen = (int)(green1 + weight * (green2 - green1));
    int interpolatedBlue = (int)(blue1 + weight * (blue2 - blue1));

    int interpolatedColor = (interpolatedRed << 16) | (interpolatedGreen << 8) | interpolatedBlue;

    return interpolatedColor;
}

int performRaycasting(t_vars *vars)
{
    if (vars->update_render == 0)
        return 0;

    if(vars->buffer_img)
        mlx_destroy_image(vars->mlx, vars->buffer_img);
    // Create a buffer image to store the rendered frame
    void *buffer_img = mlx_new_image(vars->mlx, vars->screenWidth, vars->screenHeight);
    int *buffer_data = (int *)mlx_get_data_addr(buffer_img, &(vars->bits_per_pixel), &(vars->size_line), &(vars->endian));

    // Raycasting and rendering logic
    for (vars->cast_vars->x = 0; vars->cast_vars->x < vars->screenWidth; vars->cast_vars->x++)
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
        int lineHeight = (int)(vars->screenHeight / vars->cast_vars->perpWallDist);
        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + vars->screenHeight / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + vars->screenHeight / 2;
        if (drawEnd >= vars->screenHeight)
            drawEnd = vars->screenHeight - 1;
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
        // Draw the pixels of the stripe on the buffer image
        draw_line(buffer_data, vars->cast_vars->x, drawStart, vars->cast_vars->x, drawEnd, color, vars->screenWidth);
        // Draw the pixels of the floor and ceiling on the buffer image
        int floorColor = 0x808080; // Gray color for the floor
        int ceilingColor = 0x404040; // Dark gray color for the ceiling
        for (int y = drawEnd + 1; y < vars->screenHeight; y++)
        {
            // Draw the floor and ceiling pixels on the buffer image
            buffer_data[y * vars->screenWidth + vars->cast_vars->x] = floorColor;
            buffer_data[(vars->screenHeight - y) * vars->screenWidth + vars->cast_vars->x] = ceilingColor;
        }
    }

    // Render the buffer image to the screen
    mlx_put_image_to_window(vars->mlx, vars->win, buffer_img, 0, 0);
    mlx_destroy_image(vars->mlx, buffer_img);

    vars->update_render = 0;
    return 0;
}


int handle_key_press(int keycode, t_vars *vars)
{
    double moveSpeed = 0.1;
    double rotationSpeed = 0.05;

    if (keycode == KEY_ESC)
    {
        // Close the window and quit the program
        mlx_destroy_window(vars->mlx, vars->win);
        mlx_destroy_display(vars->mlx);
        free(vars->mlx);
        exit(0);
    }
    if (keycode == KEY_W || keycode == KEY_UP)
    {
        // Move forward
        if (worldMap[(int)(vars->posX + vars->dirX * moveSpeed)][(int)(vars->posY)] == 0)
            vars->posX += vars->dirX * moveSpeed;
        if (worldMap[(int)(vars->posX)][(int)(vars->posY + vars->dirY * moveSpeed)] == 0)
            vars->posY += vars->dirY * moveSpeed;
    }
    if (keycode == KEY_A)
    {
        // Strafe left
        if (worldMap[(int)(vars->posX - vars->dirY * moveSpeed)][(int)(vars->posY)] == 0)
            vars->posX -= vars->dirY * moveSpeed;
        if (worldMap[(int)(vars->posX)][(int)(vars->posY + vars->dirX * moveSpeed)] == 0)
            vars->posY += vars->dirX * moveSpeed;
    }
    if (keycode == KEY_S || keycode == KEY_DOWN)
    {
        // Move backward
        if (worldMap[(int)(vars->posX - vars->dirX * moveSpeed)][(int)(vars->posY)] == 0)
            vars->posX -= vars->dirX * moveSpeed;
        if (worldMap[(int)(vars->posX)][(int)(vars->posY - vars->dirY * moveSpeed)] == 0)
            vars->posY -= vars->dirY * moveSpeed;
    }
    if (keycode == KEY_D)
    {
        // Strafe right
        if (worldMap[(int)(vars->posX + vars->dirY * moveSpeed)][(int)(vars->posY)] == 0)
            vars->posX += vars->dirY * moveSpeed;
        if (worldMap[(int)(vars->posX)][(int)(vars->posY - vars->dirX * moveSpeed)] == 0)
            vars->posY -= vars->dirX * moveSpeed;
    }
    if (keycode == KEY_LEFT)
    {
        // Rotate left
        double oldDirX = vars->dirX;
        vars->dirX = vars->dirX * cos(rotationSpeed) - vars->dirY * sin(rotationSpeed);
        vars->dirY = oldDirX * sin(rotationSpeed) + vars->dirY * cos(rotationSpeed);
        double oldPlaneX = vars->planeX;
        vars->planeX = vars->planeX * cos(rotationSpeed) - vars->planeY * sin(rotationSpeed);
        vars->planeY = oldPlaneX * sin(rotationSpeed) + vars->planeY * cos(rotationSpeed);
    }
    if (keycode == KEY_RIGHT)
    {
        // Rotate right
        double oldDirX = vars->dirX;
        vars->dirX = vars->dirX * cos(-rotationSpeed) - vars->dirY * sin(-rotationSpeed);
        vars->dirY = oldDirX * sin(-rotationSpeed) + vars->dirY * cos(-rotationSpeed);
        double oldPlaneX = vars->planeX;
        vars->planeX = vars->planeX * cos(-rotationSpeed) - vars->planeY * sin(-rotationSpeed);
        vars->planeY = oldPlaneX * sin(-rotationSpeed) + vars->planeY * cos(-rotationSpeed);
    }
    vars->update_render = 1;
    return (0);
}






int main(void)
{
    
    
    
    
    t_vars vars;
    Display *display = XOpenDisplay(NULL);
    Screen *screen = DefaultScreenOfDisplay(display);
    
    int width = WidthOfScreen(screen);
    int height = HeightOfScreen(screen);
    
    printf("Screen resolution: %d x %d\n", width, height);
    
    XCloseDisplay(display);
    t_cast_vars cast_vars;
    vars.posX = 22;
    vars.posY = 12;
    vars.dirX = -1;
    vars.dirY = 0;
    vars.planeX = 0;
    vars.planeY = 0.66;
    vars.buffer_img = NULL;
    vars.buffer_data = NULL;
    vars.bits_per_pixel = 0;
    vars.size_line = 0;
    vars.endian = 0;
    vars.update_render=1;
    vars.cast_vars=&cast_vars;
    vars.screenWidth=WidthOfScreen(screen)-10;
    vars.screenHeight=height-100;
    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, vars.screenWidth, vars.screenHeight, "Raycaster");

    
    // draw_frame(&vars);
    performRaycasting(&vars);
    mlx_hook(vars.win, 2, 1L << 0, handle_key_press, &vars); 
    mlx_loop_hook(vars.mlx, performRaycasting, &vars);
    mlx_loop(vars.mlx);
    return 0;
}