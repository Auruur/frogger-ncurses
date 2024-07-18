#ifndef ALL_H
#define ALL_H

//include librerie
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

//definizione macro
#define NUM_LAIRS 5
#define NUM_THREADS 19
#define NUM_VEHICLES 8
#define MAX_TRAFFIC 2
#define NUM_LANES 4
#define TIME_LEFT 30
#define BULLET_SPEED 30000

#define EASY_LIFE 5
#define MEDIUM_LIFE 3
#define HARD_LIFE 1

#define EASY_CARSPEED 90000
#define MEDIUM_CARSPEED 60000
#define HARD_CARSPEED 30000

#define EASY_SHOT 70
#define MEDIUM_SHOT 40
#define HARD_SHOT 10

#define SPACE_BAR 32

//definizione strutture
typedef struct{
    int y ;
    int x ;
    char type ;
    int id ;
    bool enemy ;
} obj;

typedef struct{
    int y ;
    int senso ;
} corsia ;

//protitipi funzione thread
void* frog_routine() ;
void* vehicle_routine(void*) ;
void* log_routine(void*) ;
void* bullet_routine() ;
void* enemy_routine() ;
void* timer_routine() ;
void* music_routine() ;

//prototipi funzioni general purpose
bool control_zone() ;
void printer() ;
void color_starter() ;
void drawmap() ;
void initialize_road_river() ;
bool menu() ;
void title_maker() ;
void difficulty_maker() ;
void graphic_frog() ;
void win() ;
void gameover() ;

#endif