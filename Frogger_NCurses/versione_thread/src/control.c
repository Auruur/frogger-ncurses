#include "all.h"
#include "global.h"

bool control_zone(){
    //inizializzo le variabili che serviranno nel controllo
    int car_result = 0 ;
    bool exit = FALSE ;
    bool win_flag = FALSE ;
    bool gameover_flag = FALSE ;
    pthread_t death_thread ;
    pthread_t point_thread ;
    //path dei suoni
    char death_sound[] = "src/sounds/death.wav" ;
    char point_sound[] = "src/sounds/win.wav" ;

    while(true){
        //conto tutti i thread e singolarmente le macchine per assicurarmi di far partire le collisioni
        //solo quando tutti sono correttamente generati
        sem_getvalue(&count, &result);
        sem_getvalue(&car_count, &car_result) ;
        if(result >= NUM_THREADS && car_result >= NUM_VEHICLES){
        
            //*****************************GESTIONE COLLISIONI************************************************************************
            
            //collisione frog_obj-->veicolo
            for(int i = 0 ; i < NUM_VEHICLES ; i++){
                if( (vehicle_obj[i].type == 'm' && vehicle_obj[i].y == frog_obj.y && ( (frog_obj.x >= vehicle_obj[i].x && frog_obj.x <= vehicle_obj[i].x+6) || (frog_obj.x+5 >= vehicle_obj[i].x && frog_obj.x+5 <= vehicle_obj[i].x+6)) || (vehicle_obj[i].type == 'c' && vehicle_obj[i].y == frog_obj.y && ( (frog_obj.x >= vehicle_obj[i].x && frog_obj.x <= vehicle_obj[i].x+10) || (frog_obj.x+5 >= vehicle_obj[i].x && frog_obj.x+5 <= vehicle_obj[i].x+10)) ) )){
                    pthread_mutex_lock(&mutex) ;
                    pthread_create(&death_thread, NULL, &music_routine, (void *)death_sound) ;
                    time_left = TIME_LEFT ;
                    life-- ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    pthread_mutex_unlock(&mutex) ;
                }
            }

            //collisione frog_obj-->acqua
            for(int i = 0 ; i < NUM_LANES ; i++){
                if(frog_obj.y == log_obj[i].y && (frog_obj.x < log_obj[i].x || frog_obj.x+4 > log_obj[i].x+9)){
                    pthread_mutex_lock(&mutex) ;
                    pthread_create(&death_thread, NULL, &music_routine, (void *)death_sound) ;
                    time_left = TIME_LEFT ;
                    life-- ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    pthread_mutex_unlock(&mutex) ;
                }
            }

            //collisione frog_obj-->nemico
            for(int i = 0 ; i < NUM_LANES ; i++){
                if(log_obj[i].enemy && frog_obj.y == log_obj[i].y){
                    pthread_mutex_lock(&mutex) ;
                    pthread_create(&death_thread, NULL, &music_routine, (void *)death_sound) ;
                    time_left = TIME_LEFT ;
                    life-- ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    pthread_mutex_unlock(&mutex) ;
                }
            }

            //collisione proiettile frog_obj-->nemico
            for(int i = 0 ; i < NUM_LANES ; i++){
                if(log_obj[i].enemy == TRUE && bullet_obj.x >= log_obj[i].x+3 && bullet_obj.x <= log_obj[i].x+6 && bullet_obj.y <= log_obj[i].y && bullet_obj.y >= log_obj[i].y-2){
                    pthread_mutex_lock(&mutex) ;
                    log_obj[i].enemy = FALSE ;
                    score += 50 ;
                    bullet_obj.x = -1 ;
                    bullet_obj.y = -1 ;
                    pthread_mutex_unlock(&mutex) ;
                }
            }
            
            //collisione proiettile nemico-->frog_obj
            for(int i = 0 ; i < NUM_LANES ; i++){
                if(enemy_bullet_obj[i].x >= frog_obj.x && enemy_bullet_obj[i].x <= frog_obj.x+4 && enemy_bullet_obj[i].y >= frog_obj.y-2 && enemy_bullet_obj[i].y <= frog_obj.y){
                    pthread_mutex_lock(&mutex) ;
                    pthread_create(&death_thread, NULL, &music_routine, (void *)death_sound) ;
                    time_left = TIME_LEFT ;
                    life-- ;
                    enemy_bullet_obj[i].x = i ;
                    enemy_bullet_obj[i].y = maxx ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    pthread_mutex_unlock(&mutex) ;
                }
            }

            //collisione proiettile nemico<-->proiettile frog_obj
            for(int i = 0 ; i < NUM_LANES ; i++){
                if(enemy_bullet_obj[i].x == bullet_obj.x && enemy_bullet_obj[i].y == bullet_obj.y){
                    pthread_mutex_lock(&mutex) ;
                    enemy_bullet_obj[i].x = i ;
                    enemy_bullet_obj[i].y = maxx ;
                    bullet_obj.x = -1 ;
                    bullet_obj.y = -1 ;
                    pthread_mutex_unlock(&mutex) ;
                }
            }
            
            //collisione proiettile frog_obj-->veicolo
            for(int i = 0 ; i < NUM_VEHICLES ; i++){
                if( bullet_obj.x >= vehicle_obj[i].x && ((vehicle_obj[i].type == 'm' && bullet_obj.x <= vehicle_obj[i].x+6) || (vehicle_obj[i].type == 'c' && bullet_obj.x <= vehicle_obj[i].x+9)) && bullet_obj.y >= vehicle_obj[i].y-2 && bullet_obj.y <= vehicle_obj[i].y){
                    pthread_mutex_lock(&mutex) ;
                    bullet_obj.x = -1 ;
                    bullet_obj.y = -1 ;
                    pthread_mutex_unlock(&mutex) ;
                }
            }

            //collisione proiettile nemico-->veicolo
            for(int i = 0 ; i < NUM_VEHICLES ; i++){
                for(int j = 0 ; j < NUM_LANES ; j++){
                    if( enemy_bullet_obj[j].x >= vehicle_obj[i].x && ((vehicle_obj[i].type == 'm' && enemy_bullet_obj[j].x <= vehicle_obj[i].x+6) || (vehicle_obj[i].type == 'c' && enemy_bullet_obj[j].x <= vehicle_obj[i].x+9)) && enemy_bullet_obj[j].y >= vehicle_obj[i].y-2 && enemy_bullet_obj[j].y <= vehicle_obj[i].y){
                        pthread_mutex_lock(&mutex) ;
                        enemy_bullet_obj[j].x = j ;
                        enemy_bullet_obj[j].y = maxx ;
                        pthread_mutex_unlock(&mutex) ;
                    }
                }
            }
            //*****************************FINE GESTIONE COLLISIONI************************************************************************

            //*****************************GESTIONE BASI********************************************************
            //per ogni base l'if esterno controlla se la rana è compresa, l'if interno se la base è già riempita
            //e l'else interno se la base è vuota e può essere riempita
            //l'else finale se l'altezza è quella delle basi ma la rana non è compresa totalmente/parzialmente
            //il completamento di una base riproduce un suono con un thread

            if(frog_obj.x >= 2 && frog_obj.x+5 <= 12 && frog_obj.y == 4){
                if(!b1_flag){
                    pthread_mutex_lock(&mutex) ;
                    time_left = TIME_LEFT ;
                    life-- ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    pthread_mutex_unlock(&mutex) ;
                }
                else{
                    pthread_mutex_lock(&mutex) ;
                    pthread_create(&point_thread, NULL, &music_routine, (void *)point_sound) ;
                    time_left = TIME_LEFT ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    score += 200 ;
                    b1_flag = FALSE ;
                    lairs_left-- ;
                    pthread_mutex_unlock(&mutex) ;
                }
            }

            if(frog_obj.x >= 19 && frog_obj.x+5 <= 29 && frog_obj.y == 4){
                if(!b2_flag){
                    pthread_mutex_lock(&mutex) ;
                    time_left = TIME_LEFT ;
                    life-- ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    pthread_mutex_unlock(&mutex) ;
                }
                else{
                    pthread_mutex_lock(&mutex) ;
                    pthread_create(&point_thread, NULL, &music_routine, (void *)point_sound) ;
                    time_left = TIME_LEFT ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    score += 200 ;
                    b2_flag = FALSE ;
                    lairs_left-- ;
                    pthread_mutex_unlock(&mutex) ;
                }
            }

            if(frog_obj.x >= 36 && frog_obj.x+5 <= 46 && frog_obj.y == 4){
                if(!b3_flag){
                    pthread_mutex_lock(&mutex) ;
                    time_left = TIME_LEFT ;
                    life-- ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    pthread_mutex_unlock(&mutex) ;
                }
                else{
                    pthread_mutex_lock(&mutex) ;
                    pthread_create(&point_thread, NULL, &music_routine, (void *)point_sound) ;
                    time_left = TIME_LEFT ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    score += 200 ;
                    b3_flag = FALSE ;
                    lairs_left-- ;
                    pthread_mutex_unlock(&mutex) ;
                }
            }

            if(frog_obj.x >= 53 && frog_obj.x+5 <= 63 && frog_obj.y == 4){
                if(!b4_flag){
                    pthread_mutex_lock(&mutex) ;
                    time_left = TIME_LEFT ;
                    life-- ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    pthread_mutex_unlock(&mutex) ;
                }
                else{
                    pthread_mutex_lock(&mutex) ;
                    pthread_create(&point_thread, NULL, &music_routine, (void *)point_sound) ;
                    time_left = TIME_LEFT ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    score += 200 ;
                    b4_flag = FALSE ;
                    lairs_left-- ;
                    pthread_mutex_unlock(&mutex) ;
                }
            }

            if(frog_obj.x >= 70 && frog_obj.x+5 <= 80 && frog_obj.y == 4){
                if(!b5_flag){
                    pthread_mutex_lock(&mutex) ;
                    time_left = TIME_LEFT ;
                    life-- ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    pthread_mutex_unlock(&mutex) ;
                }
                else{
                    pthread_mutex_lock(&mutex) ;
                    pthread_create(&point_thread, NULL, &music_routine, (void *)point_sound) ;
                    time_left = TIME_LEFT ;
                    frog_obj.x = maxx/2 ;
                    frog_obj.y = maxy-4 ;
                    score += 200 ;
                    b5_flag = FALSE ;
                    lairs_left-- ;
                    pthread_mutex_unlock(&mutex) ;
                }
            }
            //se è nella stessa y ma non nella base
            else if(frog_obj.y == 4){
                pthread_mutex_lock(&mutex) ;
                time_left = TIME_LEFT ;
                life-- ;
                frog_obj.x = maxx/2 ;
                frog_obj.y = maxy-4 ;
                pthread_mutex_unlock(&mutex) ;
            }
            //*****************************FINE GESTIONE BASI********************************************************
            
            //*****************************CONTROLLO CONDIZIONI DI FINE MANCHE/PARTITA*******************************

            //fine tempo della manche
            if(time_left <= 0){
                pthread_mutex_lock(&mutex) ;
                time_left = TIME_LEFT ;
                life-- ;
                frog_obj.x = maxx/2 ;
                frog_obj.y = maxy-4 ;
                pthread_mutex_unlock(&mutex) ;
            }

            //fine delle life-->gameover
            if(life == 0){
                exit = TRUE ;
                gameover_flag = TRUE ;
            }
            
            //basi completate-->vittoria
            if(lairs_left == 0){
                exit = TRUE ;
                win_flag = TRUE ;
            }
            //*****************************FINE CONTROLLO CONDIZIONI DI FINE MANCHE/PARTITA*******************************
        }
        //fine della partita
        if(exit){
            if(gameover_flag)
                return FALSE ;
            else if(win_flag)
                return TRUE ;
        }
    }
}