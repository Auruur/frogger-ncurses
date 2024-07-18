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
#include <string.h>

//definizione macro
#define NUM_V 8
#define MAX_TRAFFIC 2
#define NUM_T 4
#define BULLET_SPEED 30000
#define TIME_LEFT 30
#define SPACE_BAR 32
#define NUM_LAIRS 5

#define EASY_LIFE 5
#define MEDIUM_LIFE 3
#define HARD_LIFE 1

#define EASY_CARSPEED 90000
#define MEDIUM_CARSPEED 60000
#define HARD_CARSPEED 30000

#define EASY_SHOT 70
#define MEDIUM_SHOT 40
#define HARD_SHOT 10

//definizione strutture
typedef struct{
    int y ;
    int x ;
    char c ;
    char type ;
    int id ;
    bool flag ;
    bool t_flag ;
    bool enemy ;
} obj;

typedef struct{
    int y ;
    int senso ;
} corsia ;

typedef struct{
    bool result ;
    int score ;
} endval ; 

//protitipi funzioni processi
void rana(int p[2], int p_frog[2], int p_frogtobullet[2], int p_bullettofrog[2], int p_bulletcollision[2], int p_log[2]) ;
void dati_veicolo(int p_car[2], int p_coda[2]) ;
void generatrice(int p[2], int p_car[2], int p_coda[2]) ;
endval controllo(int p[2], int p_frog[2], int p_log[2], int (*p_onlog)[2], int (*p_enemy)[2], int p_bulletcollision[2], int (*p_ebullet)[2], int p_time[2], int p_reset[2]) ;
void tronco(int p[2], int (*p_onlog)[2], int (*p_enemy)[2], int p_logtobullet[2], int (*p_ebullet)[2], int id, corsia fiume) ;
void proiettile(int p[2], int p_frogtobullet[2], int p_bulletcollision[2]) ;
void enemy(int p[2], int p_logtobullet[2], int (*p_ebullet)[2]) ;
void timer(int p_time[2], int p_reset[2]) ;

//prototipi funzioni general purpose
void drawmap() ;
bool menu() ;
void title_maker(int option) ;
void difficulty_maker(int diff_param) ;
void graphic_frog() ;
bool gameover(int score) ;
bool win(int score) ;
void initialize_road_river() ;
void color_starter() ;
void music(char* sound) ;

#endif