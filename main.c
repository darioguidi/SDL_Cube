#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
Uint32 COLOR_WHITE = 0xffffffff;
#define SIZE_POINT 3
#define CORDINATE_SYSTEM_OFFSET_X SCREEN_WIDTH/2
#define CORDINATE_SYSTEM_OFFSET_Y SCREEN_HEIGHT/2

struct Point {
    int x;
    int y;
    int z;
};

double phi = 5;

void apply_rotation(double rotation_matrix[3][3], int *x,int *y,int *z)
{
    int new_x = rotation_matrix[0][0]*(*x) + rotation_matrix[0][1]*(*y) + rotation_matrix[0][2]*(*z);
    int new_y = rotation_matrix[1][0]*(*x) + rotation_matrix[1][1]*(*y) + rotation_matrix[1][2]*(*z);
    int new_z = rotation_matrix[2][0]*(*x) + rotation_matrix[2][1]*(*y) + rotation_matrix[2][2]*(*z);

    *x = new_x;
    *y = new_y;
    *z = new_z;
}

void draw_point(SDL_Renderer *renderer, int x, int y)
{
    SDL_Rect rect = (SDL_Rect) {x,y,SIZE_POINT,SIZE_POINT};
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer,&rect);
}

int draw_point_3d(SDL_Renderer *renderer, struct Point point)
{

    int x = point.x;
    int y = point.y;
    int z = point.z;

    double rotation_y[3][3] = {
        {cos(phi), 0, sin(phi)},
        {0,        1, 0       },
        {-sin(phi),0, cos(phi)}
    };
    
    // Rotazione sull’asse X
    double rotation_x[3][3] = {
        {1, 0,         0        },
        {0, cos(phi), -sin(phi)},
        {0, sin(phi),  cos(phi)}
    };
    
    // Rotazione sull’asse Z
    double rotation_z[3][3] = {
        {cos(phi), -sin(phi), 0},
        {sin(phi),  cos(phi), 0},
        {0,         0,        1}
    };
    
    apply_rotation(rotation_z, &x, &y, &z);
    apply_rotation(rotation_y, &x, &y, &z);
    apply_rotation(rotation_x, &x, &y, &z);

    int x_2d = (int)x + CORDINATE_SYSTEM_OFFSET_X;
    int y_2d = (int)y + CORDINATE_SYSTEM_OFFSET_Y;

    draw_point(renderer, x_2d, y_2d);
    return 1;
}

int draw_point_3d_array(SDL_Renderer *renderer, struct Point *point, int length){
    for(int i=0;i<length;i++){
        draw_point_3d(renderer, *(point+i));
    }
    return 1;
}

struct Point* generate_cube(int number_points)
{
    int points_per_edge = number_points / 12;
    int total_point = points_per_edge * 12;
    struct Point *points = malloc(total_point * sizeof(struct Point));
    int index = 0;

    int s = points_per_edge;

    // 8 vertici del cubo centrato sull'origine
    struct Point vertices[8] = {
        {-s/2, -s/2, -s/2}, // 0
        { s/2, -s/2, -s/2}, // 1
        { s/2,  s/2, -s/2}, // 2
        {-s/2,  s/2, -s/2}, // 3
        {-s/2, -s/2,  s/2}, // 4
        { s/2, -s/2,  s/2}, // 5
        { s/2,  s/2,  s/2}, // 6
        {-s/2,  s/2,  s/2}  // 7
    };

    // 12 spigoli: ogni coppia di indici rappresenta un segmento
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // base
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // top
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // verticali
    };

    for (int e = 0; e < 12; e++) {
        struct Point p1 = vertices[edges[e][0]];
        struct Point p2 = vertices[edges[e][1]];

        for (int i = 0; i < points_per_edge; i++) {
            float t = (float)i / points_per_edge;
            points[index++] = (struct Point){
                p1.x + (int)((p2.x - p1.x) * t),
                p1.y + (int)((p2.y - p1.y) * t),
                p1.z + (int)((p2.z - p1.z) * t)
            };
        }
    }

    return points;
}


int main(void) 
{

    int number_points;
    printf("Inserire il numero di punti necessari per rappresentare il nostro cubo\n");
    scanf("%d", &number_points);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Errore inizializzazione SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Cube",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE
    );
    if(window == NULL){
        printf("Errore nella apertura della finestra");
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL){
        printf("Errore nella creazione");
        return 1;
    }

    // Clear the screen (default color)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Set draw color for the rectangle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White

    struct Point *cube_points = generate_cube(number_points);

    SDL_Event event;
    int quit = 0;
    
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = 1;
        }
    
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    
        draw_point_3d_array(renderer, cube_points, number_points);
        SDL_RenderPresent(renderer);

        printf("%f \n",phi);
    
        phi += 0.05;
        SDL_Delay(16);
    }

    free(cube_points);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
