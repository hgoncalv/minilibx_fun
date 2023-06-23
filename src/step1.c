#include "../minilibx-linux/mlx.h"

int main()
{
    void *mlx;
    void *win;

    // Create a connection to the X server (on Linux) or initialize the application (on macOS)
    mlx = mlx_init();

    // Create a new window
    win = mlx_new_window(mlx, 800, 600, "Wolfenstein Replica");
    (void)win;
    // Event loop to keep the window open
    mlx_loop(mlx);

    return 0;
}
