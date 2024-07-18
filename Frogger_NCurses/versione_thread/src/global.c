#include "all.h"

//variabili globali
FILE*fp ;

int maxx = 0 ;
int maxy = 0 ;

int diff ;

int result = 0 ;

int life ;
int time_left = TIME_LEFT ;
int lairs_left = NUM_LAIRS ;
int score = 0;

int traffico[NUM_LANES] ;

bool b1_flag = TRUE ;
bool b2_flag = TRUE ;
bool b3_flag = TRUE ;
bool b4_flag = TRUE ;
bool b5_flag = TRUE ;

bool restart = TRUE ;

char car_sprite_h[6] = {" ____ "} ;
char car_sprite_m[6] = {"_|xx|_"} ;
char car_sprite_l[6] = {":o--o:"} ;

char cam_sprite_h[10] = {"__________"} ;
char cam_sprite_m[10] = {"| x xx x |"} ;
char cam_sprite_l[10] = {":-0----0-:"} ;

corsia c1, c2, c3, c4 ;
corsia f1, f2, f3, f4 ;

obj frog_obj ;
obj bullet_obj ;
obj enemy_bullet_obj[NUM_LANES] ;
obj vehicle_obj[NUM_VEHICLES] ;
obj log_obj[NUM_LANES] ;

sem_t count ;
sem_t car_count ;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER ;  