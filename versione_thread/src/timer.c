#include "all.h"
#include "global.h"

void* timer_routine(){

    //settiamo inizialmente la rana in posizione di spawn e gli assegnamo il tipo r(ana)
    pthread_mutex_lock(&mutex) ;
    sem_post(&count) ;
    pthread_mutex_unlock(&mutex) ;

    //ogni secondo con la sleep viene decrementata la variabile tempo
    while(true){
        time_left-- ;
        sleep(1) ;
    }
}