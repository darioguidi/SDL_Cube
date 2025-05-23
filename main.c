#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
Uint32 COLOR_WHITE = 0xffffffff;
#define SIZE_POINT 5
#define CORDINATE_SYSTEM_OFFSET_X SCREEN_WIDTH/2
#define CORDINATE_SYSTEM_OFFSET_Y SCREEN_HEIGHT/2

struct Point {
    int x;
    int y;
    int z;
};

double phi = 5;

void draw_point(SDL_Renderer *renderer, int x, int y)
{
    SDL_Rect rect = (SDL_Rect) {x,y,SIZE_POINT,SIZE_POINT};
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer,&rect);
}

int draw_point_3d(SDL_Renderer *renderer, struct Point point)
{
    double rotation_matrix[3][3] = {
        {cos(phi), 0, sin(phi)},
        {0,        1, 0       },
        {-sin(phi),0, cos(phi)}
    };

    double x = point.x;
    double y = point.y;
    double z = point.z;

    double x_rot = rotation_matrix[0][0]*x + rotation_matrix[0][1]*y + rotation_matrix[0][2]*z;
    double y_rot = rotation_matrix[1][0]*x + rotation_matrix[1][1]*y + rotation_matrix[1][2]*z;
    //double z_rot = rotation_matrix[2][0]*x + rotation_matrix[2][1]*y + rotation_matrix[2][2]*z;

    int x_2d = (int)x_rot + CORDINATE_SYSTEM_OFFSET_X;
    int y_2d = (int)y_rot + CORDINATE_SYSTEM_OFFSET_Y;

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
    int total_point = points_per_edge*12;
    struct Point *points = malloc(total_point * sizeof(struct Point));
    int index = 0;

    struct Point vertices[8] = {
        {0, 0, 0}, {points_per_edge, 0, 0}, {points_per_edge, -points_per_edge, 0}, {0, points_per_edge, 0},
        {0, 0, -points_per_edge}, {points_per_edge, 0, points_per_edge}, {points_per_edge, -points_per_edge, points_per_edge}, {0, -points_per_edge, -points_per_edge}
    };

    for(int i=0;i<points_per_edge;i++){
        points[index] = (struct Point){vertices[0].x+i,vertices[0].y,vertices[0].z};
        index++;
        points[index] = (struct Point){vertices[0].x,vertices[0].y-i,vertices[0].z};
        index++;
        points[index] = (struct Point){vertices[0].x,vertices[0].y,vertices[0].z-i};
        index++;
    }

    for(int i=0;i<points_per_edge;i++){
        points[index] = (struct Point){vertices[2].x-i,vertices[2].y,vertices[2].z};
        index++;
        points[index] = (struct Point){vertices[2].x,vertices[2].y+i,vertices[2].z};
        index++;
        points[index] = (struct Point){vertices[2].x,vertices[2].y,vertices[2].z-i};
        index++;
    }

    for(int i=0;i<points_per_edge;i++){
        points[index] = (struct Point){vertices[5].x-i,vertices[5].y,vertices[5].z};
        index++;
        points[index] = (struct Point){vertices[5].x,vertices[5].y-i,vertices[5].z};
        index++;
        points[index] = (struct Point){vertices[5].x,vertices[5].y,vertices[5].z+i};
        index++;
    }

    for(int i=0;i<points_per_edge;i++){
        points[index] = (struct Point){vertices[7].x+i,vertices[7].y,vertices[7].z};
        index++;
        points[index] = (struct Point){vertices[7].x,vertices[7].y+i,vertices[7].z};
        index++;
        points[index] = (struct Point){vertices[7].x,vertices[7].y,vertices[7].z+i};
        index++;
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
    
        phi += 0.05;
        SDL_Delay(16);
    }

    free(cube_points);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
