#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
Uint32 COLOR_WHITE = 0xffffffff;
#define SIZE_POINT 2
#define CORDINATE_SYSTEM_OFFSET_X SCREEN_WIDTH/2
#define CORDINATE_SYSTEM_OFFSET_Y SCREEN_HEIGHT/2

struct Point {
    int x;
    int y;
    int z;
};

int draw_point(SDL_Renderer *renderer, int x, int y)
{
    SDL_Rect rect = (SDL_Rect) {x,y,SIZE_POINT,SIZE_POINT};
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer,&rect);
    return 1;
}

int draw_point_3d(SDL_Renderer *renderer, struct Point point)
{
    int x_2d =  point.x + CORDINATE_SYSTEM_OFFSET_X;
    int y_2d = point.y + CORDINATE_SYSTEM_OFFSET_Y;
    draw_point(renderer,x_2d,y_2d);
    return 1;
}

int draw_point_3d_array(SDL_Renderer *renderer, struct Point *point, int length){
    for(int i=0;i<length;i++){
        draw_point_3d(renderer, *(point+i));
    }
    return 1;
}

struct Point* generate_cube(int number_points){
    struct Point *points = malloc(number_points * sizeof(struct Point));

    int points_per_edge = number_points/12;
    int index=0;

    struct Point vertices[8] = {
        {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
        {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}
    };

    int edge_pairs[12][2] = {
        {0,1}, {1,2}, {2,3}, {3,0},
        {4,5}, {5,6}, {6,7}, {7,4},
        {0,4}, {1,5}, {2,6}, {3,7}
    };



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

    struct Point point = {1,1,1};
    struct Point *cube_points = generate_cube(number_points);

    draw_point_3d(renderer,point);

    draw_point_3d_array(renderer, cube_points, number_points);

    SDL_RenderPresent(renderer);

    SDL_Event event;
    int quit = 0;
    while(!quit){
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                quit = 1;
            }
        }
        SDL_Delay(16);
    }

    free(cube_points);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
