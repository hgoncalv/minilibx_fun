#include "../minilibx-linux/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <X11/Xlib.h>

#include "../lib/libft.h"

// #define screenWidth 1024
// #define screenHeight 768


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define RECTANGLE_WIDTH 200
#define RECTANGLE_HEIGHT 150

#define KEY_ESC 0xFF1B   // ESC key
#define KEY_LEFT 0xFF51  // Left arrow key
#define KEY_RIGHT 0xFF53 // Right arrow key
#define KEY_UP 0xFF52    // Up arrow key
#define KEY_DOWN 0xFF54  // Down arrow key
#define KEY_W 0x0077     // W key
#define KEY_A 0x0061     // A key
#define KEY_S 0x0073     // S key
#define KEY_D 0x0064     // D key

#define texWidth 64
#define texHeight 64
#define NUM_TEXTURES 8

typedef struct s_mapData{
    char *north_texture;
    char *south_texture;
    char *west_texture;
    char *east_texture;
    int floor_color[3];
    int floor_color_int;
    int ceiling_color[3];
    int ceiling_color_int;
    int **map;
    int map_width;
    int map_height;
} t_mapData;

typedef struct s_cast_vars
{
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
typedef struct s_texture
{
    int width;
    int height;
    int bits_per_pixel;
    int size_line;
    int endian;
    int *data;
} t_texture;

typedef struct s_vars
{
    void *mlx;
    void *win;
    t_texture textures[8];
    t_mapData *mapData;
    
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


void draw_rectangle(void *mlx, void *win, int x, int y, int width, int height, int color);
void freeMap(t_mapData *mapData);
void addMinimap(t_vars *vars);

void	ft_matrix_free(char **matrix)
{
	int	col_num;

	col_num = 0;
	if (matrix[0] != NULL)
	{
		while (matrix[col_num] != NULL)
		{
			free(matrix[col_num]);
			col_num++;
		}
		free(matrix[col_num]);
	}
	free(matrix);
}

int	ft_matrix_get_num_col(char **matrix)
{
	int	col_num;

	col_num = -1;
	while (matrix[++col_num] != NULL)
		;
	return (col_num);
}

char	**ft_matrix_remove_col_by_index(char **matrix, int index)
{
	int		col_num;
	char	**list;
	int		i;
	int		j;

	list = NULL;
	col_num = ft_matrix_get_num_col(matrix);
	list = ft_realloc(list, (col_num) * sizeof(char *));
	i = 0;
	j = 0;
	while (i < col_num - 1)
	{
		if (i == index)
			i++;
		list[j] = ft_strdup(matrix[i]);
		i++;
		j++;
	}
	list[j] = NULL;
	ft_matrix_free(matrix);
	return (list);
}


char	**ft_matrix_dup(char **matrix, int push)
{
	int		col_num;
	char	**list;
	int		i;

	list = NULL;
	col_num = ft_matrix_get_num_col(matrix);
	list = ft_realloc(list, (col_num + 1 + push) * sizeof(char *));
	i = -1;
	while (++i < col_num)
		list[i] = ft_strdup(matrix[i]);
	list[i + push] = NULL;
	return (list);
}

char	**ft_matrix_push(char **matrix, char *str_getsdeleted)
{
	int		col_num;
	char	**list;
	char	*str;

	str = str_getsdeleted;
	col_num = ft_matrix_get_num_col(matrix);
	list = ft_matrix_dup(matrix, 1);
	list[col_num] = ft_strdup(str);
	free(str);
	ft_matrix_free(matrix);
	return (list);
}

void	ft_print_matrix(char **matrix)
{
	int		i;
	

	i = 0;
	while (matrix[i] != NULL)
	{
		printf("%s\n",matrix[i]);
		i++;
	}
	return ;
}


void draw_line(t_vars *vars, int x1, int y1, int x2, int y2, int color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (x1 != x2 || y1 != y2)
    {
        // Set pixel at (x1, y1) to the desired color
        mlx_pixel_put(vars->mlx, vars->win, x1, y1, color);

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
    }
}

void draw_rectangle(void *mlx, void *win, int x, int y, int width, int height, int color)
{
    //0xFFFFFF
    int endX = x + width;
    int endY = y + height;

    // Draw vertical lines
    for (int i = y; i < endY; i++)
    {
        mlx_pixel_put(mlx, win, x, i, color);     // Set pixel color to white
        mlx_pixel_put(mlx, win, endX, i, color);  // Set pixel color to white
    }

    // Draw horizontal lines
    for (int i = x; i < endX; i++)
    {
        mlx_pixel_put(mlx, win, i, y, color);     // Set pixel color to white
        mlx_pixel_put(mlx, win, i, endY, color);  // Set pixel color to white
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
        if (vars->cast_vars->mapX < 0 || vars->cast_vars->mapX >= vars->mapData->map_width || vars->cast_vars->mapY < 0 || vars->cast_vars->mapY >= vars->mapData->map_height)
            break;
        // Check if ray has hit a wall
        if (vars->mapData->map[vars->cast_vars->mapX][vars->cast_vars->mapY] > 0)
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

t_texture loadTexture(const char *filename, t_vars *vars)
{
    t_texture texture;
    texture.width = 0;
    texture.height = 0;
    texture.data = NULL;

    

    void *image = mlx_xpm_file_to_image(vars->mlx, (char *)filename, &texture.width, &texture.height);
    if (!image)
    {
        printf("Failed to load XPM file: %s\n", filename);
        // mlx_destroy_display(vars->mlx);
        return texture;
    }

    int *image_data = (int *)mlx_get_data_addr(image, &texture.bits_per_pixel, &texture.size_line, &texture.endian);

    texture.data = (int *)malloc(sizeof(int) * texture.width * texture.height);
    if (!texture.data)
    {
        printf("Failed to allocate memory for texture data\n");
        mlx_destroy_image(vars->mlx, image);
        // mlx_destroy_display(vars->mlx);
        return texture;
    }

    // Copy the image data to the texture data
    memcpy(texture.data, image_data, sizeof(int) * texture.width * texture.height);

    mlx_destroy_image(vars->mlx, image);
    // mlx_destroy_display(vars->mlx);

    return texture;
}

int performRaycasting(t_vars *vars)
{
    if (vars->update_render == 0)
        return 0;

    if (vars->buffer_img)
        mlx_destroy_image(vars->mlx, vars->buffer_img);

    // Add more texture loading as needed

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
        // Choose wall texture based on the tile type
        int texNum;
        if (vars->cast_vars->side == 0 && vars->cast_vars->rayDirX < 0)
            texNum = 3; // West wall
        else if (vars->cast_vars->side == 0 && vars->cast_vars->rayDirX > 0)
            texNum = 2; // East wall
        else if (vars->cast_vars->side == 1 && vars->cast_vars->rayDirY < 0)
            texNum = 1; // South wall
        else
            texNum = 0; // North wall

        // Calculate the texture coordinate (x-coordinate on the texture)
        double wallX;
        if (vars->cast_vars->side == 0)
            wallX = vars->posY + vars->cast_vars->perpWallDist * vars->cast_vars->rayDirY;
        else
            wallX = vars->posX + vars->cast_vars->perpWallDist * vars->cast_vars->rayDirX;
        wallX -= floor(wallX);
        int texX = (int)(wallX * (vars->textures)[texNum].width);
        if (vars->cast_vars->side == 0 && vars->cast_vars->rayDirX > 0)
            texX = (vars->textures)[texNum].width - texX - 1;
        if (vars->cast_vars->side == 1 && vars->cast_vars->rayDirY < 0)
            texX = (vars->textures)[texNum].width - texX - 1;
        // Draw the pixels of the stripe using the texture
        for (int y = drawStart; y < drawEnd; y++)
        {
            // Calculate the texture coordinate (y-coordinate on the texture)
            int texY = (((y * 2 - vars->screenHeight + lineHeight) << 6) / lineHeight) / 2;
            // Calculate the texture color based on the texture number and texture coordinates
            int color = (vars->textures)[texNum].data[(vars->textures)[texNum].width * texY + texX];
            
            //interpolate the color
            
            // Draw the color to the buffer image
            buffer_data[y * vars->screenWidth + vars->cast_vars->x] = color;
        }
        // Draw the pixels of the floor and ceiling on the buffer image
        int floorColor = vars->mapData->floor_color_int;   // Gray color for the floor
        int ceilingColor = vars->mapData->ceiling_color_int; // Dark gray color for the ceiling
        for (int y = drawEnd + 1; y < vars->screenHeight; y++)
        {
            // Draw the floor and ceiling pixels on the buffer image
            buffer_data[y * vars->screenWidth + vars->cast_vars->x] = floorColor;
            buffer_data[(vars->screenHeight - y) * vars->screenWidth + vars->cast_vars->x-1] = ceilingColor;
        }
    }

    // Render the buffer image to the screen
    mlx_put_image_to_window(vars->mlx, vars->win, buffer_img, 0, 0);
    mlx_destroy_image(vars->mlx, buffer_img);

    // Add minimap
    addMinimap(vars);

    vars->update_render = 0;
    return 0;
}


void addMinimap(t_vars *vars)
{
    // Calculate the size of the minimap
    int minimapSize = vars->screenHeight / 3;
    int margin = 1;

    // Calculate the position and size of the minimap rectangle
    int rectX = vars->screenWidth - minimapSize - margin;
    int rectY = vars->screenHeight - minimapSize - margin; // Updated to be at the bottom right
    // int rectWidth = minimapSize;
    // int rectHeight = minimapSize;

    // Calculate the size of each minimap tile
    int tileSize = minimapSize / vars->mapData->map_width;

    // Draw the minimap rectangle
    // draw_rectangle(vars->mlx, vars->win, rectX, rectY, rectWidth, rectHeight, 0xFFFFFF);

    // Draw the minimap tiles
    for (int i = 0; i < vars->mapData->map_height; i++)
    {
        for (int j = 0; j < vars->mapData->map_width; j++)
        {
            int tileX = rectX + i * tileSize;
            int tileY = rectY + j * tileSize;

            if (vars->mapData->map[i][j] > 0)
            {
                // Draw a filled rectangle for walls
                draw_rectangle(vars->mlx, vars->win, tileX, tileY, tileSize, tileSize, 0xFF0000);
            }
        }
    }

    // Draw the player position on the minimap
    int playerX = rectX + (int)(vars->posX * tileSize);
    int playerY = rectY + (int)(vars->posY * tileSize);
    draw_rectangle(vars->mlx, vars->win, playerX, playerY, tileSize, tileSize, 0x0000FF);

    // Calculate the direction line position and length
    int lineX = playerX + tileSize / 2;
    int lineY = playerY + tileSize / 2;
    int lineLength = tileSize / 2;

    // Calculate the end point of the direction line based on player direction
    double playerDirectionX = vars->dirX * lineLength;
    double playerDirectionY = vars->dirY * lineLength;
    int lineEndX = lineX + playerDirectionX;
    int lineEndY = lineY + playerDirectionY;

    // Draw the direction line
    draw_line(vars, lineX, lineY, lineEndX, lineEndY, 0xFFFF00);
    draw_line(vars, lineX - playerDirectionY, lineY + playerDirectionX, lineEndX - playerDirectionY, lineEndY + playerDirectionX, 0x00FFFF);
    draw_line(vars, lineX + playerDirectionY, lineY - playerDirectionX, lineEndX + playerDirectionY, lineEndY - playerDirectionX, 0x00FFFF);
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
        // Free the memory allocated for the texture data
        for (int i = 0; i < NUM_TEXTURES; i++)
        {
            // mlx_destroy_image(vars->mlx, (vars->textures)[i].data);
            free((vars->textures)[i].data);
            
        }
        freeMap(vars->mapData);
        free(vars->mlx);
        exit(0);
    }
    if (keycode == KEY_W || keycode == KEY_UP)
    {
        // Move forward
        if (vars->mapData->map[(int)(vars->posX + vars->dirX * moveSpeed)][(int)(vars->posY)] == 0)//check for walls
            vars->posX += vars->dirX * moveSpeed;
        if (vars->mapData->map[(int)(vars->posX)][(int)(vars->posY + vars->dirY * moveSpeed)] == 0)//check for walls
            vars->posY += vars->dirY * moveSpeed;
    }
    if (keycode == KEY_D)
    {
        // Strafe left
        if (vars->mapData->map[(int)(vars->posX - vars->dirY * moveSpeed)][(int)(vars->posY)] == 0)
            vars->posX -= vars->dirY * moveSpeed;
        if (vars->mapData->map[(int)(vars->posX)][(int)(vars->posY + vars->dirX * moveSpeed)] == 0)
            vars->posY += vars->dirX * moveSpeed;
    }
    if (keycode == KEY_S || keycode == KEY_DOWN)
    {
        // Move backward
        if (vars->mapData->map[(int)(vars->posX - vars->dirX * moveSpeed)][(int)(vars->posY)] == 0)
            vars->posX -= vars->dirX * moveSpeed;
        if (vars->mapData->map[(int)(vars->posX)][(int)(vars->posY - vars->dirY * moveSpeed)] == 0)
            vars->posY -= vars->dirY * moveSpeed;
    }
    if (keycode == KEY_A)
    {
        // Strafe right
        if (vars->mapData->map[(int)(vars->posX + vars->dirY * moveSpeed)][(int)(vars->posY)] == 0)
            vars->posX += vars->dirY * moveSpeed;
        if (vars->mapData->map[(int)(vars->posX)][(int)(vars->posY - vars->dirX * moveSpeed)] == 0)
            vars->posY -= vars->dirX * moveSpeed;
    }
    if (keycode == KEY_RIGHT)
    {
        // Rotate left
        double oldDirX = vars->dirX;
        vars->dirX = vars->dirX * cos(rotationSpeed) - vars->dirY * sin(rotationSpeed);
        vars->dirY = oldDirX * sin(rotationSpeed) + vars->dirY * cos(rotationSpeed);
        double oldPlaneX = vars->planeX;
        vars->planeX = vars->planeX * cos(rotationSpeed) - vars->planeY * sin(rotationSpeed);
        vars->planeY = oldPlaneX * sin(rotationSpeed) + vars->planeY * cos(rotationSpeed);
    }
    if (keycode == KEY_LEFT)
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

void cleanup_textures(t_texture *textures, int numTextures)
{
    for (int i = 0; i < numTextures; i++)
    {
        free(textures[i].data);
    }
}

int convertColorToInt(int color[3]) {
    int red = color[0];
    int green = color[1];
    int blue = color[2];

    // Perform bit shifting and bitwise OR operations to combine the RGB components into a single integer color
    int intColor = (red << 16) | (green << 8) | blue;

    return intColor;
}

void parseMap(const char *filename, t_mapData *mapData) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(1);
    }

    // Initialize mapData structure
    mapData->north_texture = NULL;
    mapData->south_texture = NULL;
    mapData->west_texture = NULL;
    mapData->east_texture = NULL;
    memset(mapData->floor_color, 0, sizeof(mapData->floor_color));
    memset(mapData->ceiling_color, 0, sizeof(mapData->ceiling_color));
    mapData->map = NULL;
    mapData->map_width = 0;
    mapData->map_height = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // Skip empty lines and comments
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }

        // Parse elements based on their identifier

        // North texture
        if (strncmp(line, "NO ", 3) == 0) {
            mapData->north_texture = strdup(line + 3);
        }

        // South texture
        else if (strncmp(line, "SO ", 3) == 0) {
            mapData->south_texture = strdup(line + 3);
        }

        // West texture
        else if (strncmp(line, "WE ", 3) == 0) {
            mapData->west_texture = strdup(line + 3);
        }

        // East texture
        else if (strncmp(line, "EA ", 3) == 0) {
            mapData->east_texture = strdup(line + 3);
        }

        // Floor color
        else if (strncmp(line, "F ", 2) == 0) {
            sscanf(line + 2, "%d,%d,%d",
                   &mapData->floor_color[0],
                   &mapData->floor_color[1],
                   &mapData->floor_color[2]);
            mapData->floor_color_int = convertColorToInt(mapData->floor_color);
        }

        // Ceiling color
        else if (strncmp(line, "C ", 2) == 0) {
            sscanf(line + 2, "%d,%d,%d",
                   &mapData->ceiling_color[0],
                   &mapData->ceiling_color[1],
                   &mapData->ceiling_color[2]);
            mapData->ceiling_color_int = convertColorToInt(mapData->ceiling_color);
        }

        // Check if we have reached the map section
        else if (line[0] >= '0' && line[0] <= '9') {
            break;
        }
    }

    // Count the number of map rows and determine the map width
    int max_row_width = 0;
    int map_rows = 0;
    int map_columns = 0;

    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // Skip empty lines and comments
        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }

        int line_length = strlen(line);
        if (line_length > max_row_width) {
            max_row_width = line_length;
        }

        // Increment the number of map rows
        map_rows++;

        // Check if we have reached the end of the map
        if (strcmp(line, "") == 0) {
            break;
        }

        // Store the maximum number of columns encountered
        if (line_length > map_columns) {
            map_columns = line_length;
        }
    }

    // Allocate memory for the map array
    // mapData->map = (int **)malloc(sizeof(int *) * map_rows);

    // // Reset file pointer to the beginning of the map section
    // fseek(file, 0, SEEK_SET);

    // // Skip lines until the map section
    // while (fgets(line, sizeof(line), file)) {
    //     // Remove trailing newline character
    //     line[strcspn(line, "\n")] = '\0';

    //     // Check if we have reached the map section
    //     if (line[0] >= '0' && line[0] <= '9') {
    //         break;
    //     }
    // }

    // // Parse the map rows
    // int row_index = 0;
    // while (fgets(line, sizeof(line), file)) {
    //     // Remove trailing newline character
    //     line[strcspn(line, "\n")] = '\0';

    //     // Skip empty lines and comments
    //     if (strlen(line) == 0 || line[0] == '#') {
    //         continue;
    //     }

    //     int line_length = strlen(line);

    //     // Allocate memory for the row of the map
    //     mapData->map[row_index] = (int *)malloc(sizeof(int) * map_columns);

    //     // Parse the row of the map
    //     for (int col_index = 0; col_index < map_columns; col_index++) {
    //         if (col_index < line_length) {
    //             if (line[col_index] == 'N') {
    //                 mapData->map[row_index][col_index] = -1;
    //             }
    //             else if (line[col_index] == 'S') {
    //                 mapData->map[row_index][col_index] = -2;
    //             }
    //             else if (line[col_index] == 'E') {
    //                 mapData->map[row_index][col_index] = -3;
    //             }
    //             else if (line[col_index] == 'F') {
    //                 mapData->map[row_index][col_index] = -4;
    //             } else if (line[col_index] == ' ') {
    //                 mapData->map[row_index][col_index] = 1;
    //             } else {
    //                 mapData->map[row_index][col_index] = line[col_index] - '0';
    //             }
    //         } else {
    //             mapData->map[row_index][col_index] = 1;
    //         }
    //     }

    //     row_index++;

    //     // Check if we have parsed all rows of the map
    //     if (row_index == map_rows) {
    //         break;
    //     }
    // }

    // Set the map width and height
    mapData->map_width = map_columns;
    mapData->map_height = map_rows+1;

    fclose(file);
}




void printMap(const t_mapData *mapData) {
    printf("North Texture: %s\n", mapData->north_texture);
    printf("South Texture: %s\n", mapData->south_texture);
    printf("West Texture: %s\n", mapData->west_texture);
    printf("East Texture: %s\n", mapData->east_texture);
    printf("Floor Color: %d,%d,%d\n", mapData->floor_color[0], mapData->floor_color[1], mapData->floor_color[2]);
    printf("Ceiling Color: %d,%d,%d\n", mapData->ceiling_color[0], mapData->ceiling_color[1], mapData->ceiling_color[2]);
     printf("Map height: %d  Map width: %d \n", mapData->map_height,mapData->map_width);
    printf("Map:\n");
    for (int i = 0; i < mapData->map_height; i++) {
        for (int j = 0; j < mapData->map_width; j++) {
            printf("%d ", mapData->map[i][j]);
        }
        printf("\n");
    }
}

void freeMap(t_mapData *mapData) {
    free(mapData->north_texture);
    free(mapData->south_texture);
    free(mapData->west_texture);
    free(mapData->east_texture);

    for (int i = 0; i < mapData->map_height; i++) {
        free(mapData->map[i]);
    }
    free(mapData->map);

    // Reset mapData structure
    mapData->north_texture = NULL;
    mapData->south_texture = NULL;
    mapData->west_texture = NULL;
    mapData->east_texture = NULL;
    memset(mapData->floor_color, 0, sizeof(mapData->floor_color));
    memset(mapData->ceiling_color, 0, sizeof(mapData->ceiling_color));
    mapData->map = NULL;
    mapData->map_width = 0;
    mapData->map_height = 0;
}

void setPlayerPosition(t_vars *vars) {
    for (int x = 0; x < vars->mapData->map_width; x++) {
        for (int y = 0; y < vars->mapData->map_height; y++) {
            if (vars->mapData->map[y][x] < 0 ||vars->mapData->map[y][x] > 2) {
                printf("\nposY=%d posX=%d\n",y,x);
                vars->posX = (double)x; // Add 0.5 to center the position in the cell
                vars->posY = (double)y;//+ 0.5; // Add 0.5 to center the position in the cell
                
                if(vars->mapData->map[y][x]==30)//N
                {
                    vars->posX+=0.1;
                    vars->posY-=0.1;
                    vars->dirX = 0;
                    vars->dirY = -1;
                    vars->planeX = 0.66;
                    vars->planeY = 0;
                }
                if(vars->mapData->map[y][x]==35)//S
                {
                    vars->dirX = 0;
                    vars->dirY = 1;
                    vars->planeX = -0.66;
                    vars->planeY = 0;
                }
                if(vars->mapData->map[y][x]==21)//E
                {
                    vars->dirX = 1;
                    vars->dirY = 0;
                    vars->planeX = 0;
                    vars->planeY = 0.66;
                }
                if(vars->mapData->map[y][x]==39)//W
                {
                    vars->dirX = -1;
                    vars->dirY = 0;
                    vars->planeX = 0;
                    vars->planeY = -0.66;
                }
                vars->mapData->map[y][x] = 0;
                return; // Exit the function after setting the position
            }
        }
    }

    // Handle the case when no -1 element is found in the map
    fprintf(stderr, "No player position (-1) found in the map.\n");
    exit(1);
}

char **readLinesFromFile(const char *filename, int *lineCount)
{
    int fd;
    char *line;
    int count = 0;
    int maxLines = 1000;
    char **lines = malloc(maxLines * sizeof(char *));

    if (lines == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Open the file
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        free(lines);
        return NULL;
    }

    // Read lines until the end of the file
    int ret;
    while ((ret = ft_gnl(fd, &line)) > 0)
    {
        // if (count >= maxLines)
        // {
        //     // Increase the capacity of the lines array if needed
        //     maxLines *= 2;
        //     char **newLines = realloc(lines, maxLines * sizeof(char *));
        //     if (newLines == NULL)
        //     {
        //         fprintf(stderr, "Memory reallocation failed\n");
        //         for (int i = 0; i < count; i++)
        //             free(lines[i]);
        //         free(lines);
        //         close(fd);
        //         return NULL;
        //     }
        //     lines = newLines;
        // }

        lines[count] = line;
        count++;
        // line = NULL; // Reset line to NULL to avoid double free
    }
    lines[count] = line;
    count++;
    lines[count] = NULL;
    // Check the return value of get_next_line for errors or end of file
    if (ret == -1)
    {
        perror("Error reading file");
        for (int i = 0; i < count; i++)
            free(lines[i]);
        free(lines);
        close(fd);
        return NULL;
    }

    // Close the file
    close(fd);

    // Set the lineCount and return the lines array
    *lineCount = count;
    return lines;
}

char ** map_from_cub_matrix(char **file_matrix)
{
    char **map = ft_matrix_dup(file_matrix+6,0);
    int map_len = 0;
    //Get maximum len of map
    for (int i=0;map[i]!=NULL;i++)
    {
        if (map_len < ft_strlen(map[i]))
            map_len = ft_strlen(map[i]);
    }
    for (int i = 0;map[i]!=NULL;i++)
    {
        if (map_len > ft_strlen(map[i]))
        {
            int k = 0;
            while (map[i][k]!= '\0')
            {
                if(map[i][k++] == ' ')
                    map[i][k-1]='1'; //field spaces with borders
            }
            char *new_line = NULL;
            new_line = ft_realloc(new_line,(map_len+1)* sizeof(char) );
            int j = 0;
            while (map[i][j]!= '\0')
            {
                new_line[j]=map[i][j];
                j++;
            }
            while (j < map_len){
                new_line[j]='1';//add border to emmpty fields
                j++;
            }
            new_line[j]='\0';
            free(map[i]);
            map[i]=new_line;

        }
    }
    return (map);
}

int** convertStringArrayToIntArray(char** strings) {
    int numStrings = 0;
    while (strings[numStrings] != NULL)
        numStrings++;

    int numRows = numStrings;
    int numCols = strlen(strings[0]);

    int** intArray = (int**)malloc(numRows * sizeof(int*));
    for (int i = 0; i < numRows; i++) {
        intArray[i] = (int*)malloc(numCols * sizeof(int));
        for (int j = 0; j < numCols; j++) {
            intArray[i][j] = strings[i][j] - '0'; // Convert character to integer
        }
    }
    
    return intArray;
}


bool hasAdjacentOne(char** strings, int numRows, int numCols, int row, int col) {
    // Check left
    for (int c = col - 1; c >= 0; c--) {
        if (strings[row][c] == '1')
            return true;
        if (strings[row][c] != '1')
            break;
    }

    // Check right
    for (int c = col + 1; c < numCols; c++) {
        if (strings[row][c] == '1')
            return true;
        if (strings[row][c] != '1')
            break;
    }

    // Check up
    for (int r = row - 1; r >= 0; r--) {
        if (strings[r][col] == '1')
            return true;
        if (strings[r][col] != '1')
            break;
    }

    // Check down
    for (int r = row + 1; r < numRows; r++) {
        if (strings[r][col] == '1')
            return true;
        if (strings[r][col] != '1')
            break;
    }

    return false;
}


bool checkAdjacentOnes(char** strings, int numRows, int numCols) {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            if (strings[row][col] != '1') {
                if (!hasAdjacentOne(strings, numRows, numCols, row, col)) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main(void)
{
    char **cub_matrix;
    int lineCount;

    cub_matrix = readLinesFromFile("map.cub", &lineCount);
    if (cub_matrix == NULL)
    {
        fprintf(stderr, "Failed to read lines from file\n");
        return 1;
    }

    char ** map = map_from_cub_matrix(cub_matrix);
    int **int_map = convertStringArrayToIntArray(map);
    // printf("\n %d", int_map[0][0]);
    
    //ft_print_matrix(map);
    ft_matrix_free(cub_matrix);
    

    t_vars vars;
    Display *display = XOpenDisplay(NULL);
    Screen *screen = DefaultScreenOfDisplay(display);

    int width = WidthOfScreen(screen);
    int height = HeightOfScreen(screen);

    printf("Screen resolution: %d x %d\n", width, height);

    XCloseDisplay(display);
    t_cast_vars cast_vars;
    
    

    vars.buffer_img = NULL;
    vars.buffer_data = NULL;
    vars.bits_per_pixel = 0;
    vars.size_line = 0;
    vars.endian = 0;
    vars.update_render = 1;
    vars.cast_vars = &cast_vars;
    vars.screenWidth = width/4;
    vars.screenHeight = height/4;
    // Load textures from XPM files and store them in an array
    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, vars.screenWidth, vars.screenHeight, "Raycaster");

    t_mapData mapData;
    parseMap("map.cub", &mapData);
    vars.mapData=&mapData;
    bool result = checkAdjacentOnes(map, mapData.map_width, mapData.map_height);
    if (result) {
        printf("All non-'1' characters have at least one '1' adjacent.\n");
    } else {
        printf("There is a non-'1' character without adjacent '1's.\n");
    }

    vars.textures[0] = loadTexture(vars.mapData->north_texture, &vars);
    vars.textures[1] = loadTexture(vars.mapData->south_texture, &vars);
    vars.textures[2] = loadTexture(vars.mapData->east_texture, &vars);
    vars.textures[3] = loadTexture(vars.mapData->west_texture, &vars);
    vars.textures[4] = loadTexture("xpm/mossy.xpm", &vars);
    vars.textures[5] = loadTexture("xpm/pillar.xpm", &vars);
    vars.textures[6] = loadTexture("xpm/purplestone.xpm", &vars);
    vars.textures[7] = loadTexture("xpm/wood.xpm", &vars);
    vars.mapData->map=int_map;
    printMap(vars.mapData);
    setPlayerPosition(&vars);
    // performRaycasting(&vars);
    mlx_hook(vars.win, 2, 1L << 0, handle_key_press, &vars);
    mlx_loop_hook(vars.mlx, performRaycasting, &vars);
    mlx_loop(vars.mlx);
    
    return 0;
}