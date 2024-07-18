#include "all.h"
#include "global.h"

void* bullet_routine(){
    //aumentiamo un semaforo per segnalare che il thread è stato creato e inizializzato correttamente
    pthread_mutex_lock(&mutex) ;
    sem_post(&count) ;
    pthread_mutex_unlock(&mutex) ;
    while(true){
        //se il proiettile è compreso nello schermo, esso sale lungo l'asse verticale
        if(bullet_obj.y >= 0){
            pthread_mutex_lock(&mutex) ;
            bullet_obj.y-- ;
            pthread_mutex_unlock(&mutex) ;
            usleep(BULLET_SPEED) ;
        }
    }
}