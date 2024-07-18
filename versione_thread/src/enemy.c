#include "all.h"
#include "global.h"

void* enemy_routine(void*k){
    int id = *((int *) k) ;

    //aumentiamo un semaforo per segnalare che il thread è stato creato e inizializzato correttamente
    pthread_mutex_lock(&mutex) ;
    sem_post(&count) ;
    pthread_mutex_unlock(&mutex) ;

    //se il proiettile del nemico è compreso nello schermo viene fatto scendere verso il basso
    while(true){
        if(enemy_bullet_obj[id].y <= maxx-1){
            pthread_mutex_lock(&mutex) ;
            enemy_bullet_obj[id].y++ ;
            pthread_mutex_unlock(&mutex) ;
            usleep(BULLET_SPEED) ;
        }
    }
}