#include "all.h"

//extern delle variabili globali
extern FILE*fp ;

extern int maxx ;
extern int maxy ;

extern int diff ;

extern int result ;

extern int life ;
extern int time_left ;
extern int lairs_left  ;
extern int score ;

extern int traffico[NUM_LANES] ;

extern bool b1_flag ;
extern bool b2_flag ;
extern bool b3_flag ;
extern bool b4_flag ;
extern bool b5_flag ;

extern bool restart ;

extern char car_sprite_h[6] ;
extern char car_sprite_m[6] ;
extern char car_sprite_l[6] ;

extern char cam_sprite_h[10] ;
extern char cam_sprite_m[10] ;
extern char cam_sprite_l[10] ;

extern corsia c1, c2, c3, c4 ;
extern corsia f1, f2, f3, f4 ;

extern obj frog_obj ;
extern obj bullet_obj ;
extern obj enemy_bullet_obj[NUM_LANES] ;
extern obj vehicle_obj[NUM_VEHICLES] ;
extern obj log_obj[NUM_LANES] ;

extern sem_t count ;
extern sem_t car_count ;

extern pthread_mutex_t mutex ;  