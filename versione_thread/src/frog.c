#include "all.h"
#include "global.h"

void* frog_routine(){
    int car_result = 0 ;
    pthread_t sound_thread ;
    //path del suono di sparo della rana
    char frog_sound[] = "src/sounds/frogsound.wav" ;

    //settiamo inizialmente la rana in posizione di spawn e gli assegnamo il tipo r(ana)
    pthread_mutex_lock(&mutex) ;
    frog_obj.y = maxy-4 ;
    frog_obj.x = maxx/2 ;
    frog_obj.type = 'r' ;
    pthread_mutex_unlock(&mutex) ;

    //aumentiamo un semaforo per segnalare che il thread è stato creato e inizializzato correttamente
    pthread_mutex_lock(&mutex) ;
    sem_post(&count) ;
    pthread_mutex_unlock(&mutex) ;

    while(true){
        //facciamo la stampa nella funzione rana perchè sincronizza la stampa dei thread a tempo di input/timeout e non viene intasata da segnali continui
        sem_getvalue(&car_count, &car_result) ;
        if(car_result >= NUM_VEHICLES){
            pthread_mutex_lock(&mutex) ;
            printer() ;
            pthread_mutex_unlock(&mutex) ;
        }

        //facciamo un timeout in modo da mandare avanti il ciclo anche senza input (necessario per le collisioni)
        timeout(1) ;

        //prendiamo un input da tastiera e facciamo il movimento corrispondente, la barra spaziatrice spara un proiettile
        int s = getch() ;
        switch(s){
            case KEY_UP:
                if(frog_obj.y > 5){
                    pthread_mutex_lock(&mutex) ;
                    frog_obj.y -= 3;
                    pthread_mutex_unlock(&mutex) ;
                }
                break;
            case KEY_DOWN:
                if(frog_obj.y < maxy-4){
                    pthread_mutex_lock(&mutex) ;
                    frog_obj.y += 3;
                    pthread_mutex_unlock(&mutex) ;
                }
                break;
            case KEY_LEFT:
                if(frog_obj.x > 1){
                    pthread_mutex_lock(&mutex) ;
                    frog_obj.x -= 1;
                    pthread_mutex_unlock(&mutex) ;
                }
                break;
            case KEY_RIGHT:
                if(frog_obj.x+4 < maxx-2){
                    pthread_mutex_lock(&mutex) ;
                    frog_obj.x += 1;
                    pthread_mutex_unlock(&mutex) ;
                }
                break;
            //se viene premuta barra spaziatrice viene portato il proiettile sopra la rana,
            //permettendo alla funzione thread di operare nel suo while,
            //solo se questo non è già nello schermo (non consentisce lo spam di proiettili),
            //inoltre viene creato un thread che riproduce un suono
            case SPACE_BAR:
                if(bullet_obj.y < 0){
                    pthread_mutex_lock(&mutex) ;
                    pthread_create(&sound_thread, NULL, &music_routine, (void *)frog_sound) ;
                    bullet_obj.x = frog_obj.x+2 ;
                    bullet_obj.y = frog_obj.y-3 ;
                    pthread_mutex_unlock(&mutex) ;
                }
                break ;
        }
    }
}