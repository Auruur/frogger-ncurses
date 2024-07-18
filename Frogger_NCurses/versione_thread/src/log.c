#include "all.h"
#include "global.h"

void* log_routine(void*k){
    //settiamo l'id passato nel controllo
    int id = *((int *) k) ;
    srand(id) ;
    log_obj[id].id = id ;
    int direzione ;
    log_obj[id].enemy = FALSE ;
    //questi due timer regolano il tempo di spawn e di sparo dei nemici,
    //il secondo cambia in base alla difficoltà
    int enemy_timer = 120+rand()%300 ;
    int shoot_timer ;
    switch(diff){
        case 1:
            shoot_timer = EASY_SHOT ;
            break ;
        case 2:
            shoot_timer = MEDIUM_SHOT ;
            break ;
        case 3:
            shoot_timer = HARD_SHOT ;
            break ;
    }
    pthread_t laser_thread ;
    //path del suono di sparo del nemico
    char laser_sound[] = "src/sounds/laser.wav" ;

    //inizializzazione flusso
    switch(id){
        case 0:
            log_obj[id].y = f1.y ;
            direzione = f1.senso ;
            break ;
        case 1:
            log_obj[id].y = f2.y ;
            direzione = f2.senso ;
            break ;
        case 2:
            log_obj[id].y = f3.y ;
            direzione = f3.senso ;
            break ;
        case 3:
            log_obj[id].y = f4.y ;
            direzione = f4.senso ;
            break ;
    }

    //il tronco parte dal centro dello schermo
    log_obj[id].x = maxx/2-5 ;

    //aumentiamo un semaforo per segnalare che il thread è stato creato e inizializzato correttamente
    pthread_mutex_lock(&mutex) ;
    sem_post(&count) ;
    pthread_mutex_unlock(&mutex) ;

    //ciclo di movimento
    while(true){
        //se tocca un bordo inverte la sua direzione
        if(log_obj[id].x == 0 || log_obj[id].x+9 == maxx-1)
            direzione *= -1 ;

        //si muove della sua direzione (+1/-1)
        log_obj[id].x += direzione ;

        //se la rana è sul tronco, si muove con esso
        if(frog_obj.y == log_obj[id].y)
            frog_obj.x += direzione ;

        //se non c'è il nemico, il relativo timer di spawn decrementa
        if(log_obj[id].enemy == FALSE)
            enemy_timer-- ;
        
        //se c'è il nemico il timer di sparo decrementa
        if(log_obj[id].enemy == TRUE)
            shoot_timer-- ; 

        //se il timer di spawn è 0 e la rana non è sul tronco, una flag determina lo spawn del nemico
        if(enemy_timer <= 0 && !(frog_obj.y == log_obj[id].y)){
            enemy_timer = 120 + rand()%1000 ;
            log_obj[id].enemy = TRUE ;
        }  

        //se c'è il nemico e il timeri di sparo è 0 e il proiettile è non è già nello schermo,
        //il proiettile viene messo nello schermo, e la sua funzione thread lo farà muovere.
        //inoltre viene riprodotto con un thread il suono di sparo.
        //il timer di sparo viene risettato in base alla difficoltà
        if(log_obj[id].enemy == TRUE && enemy_bullet_obj[id].y > maxx-1 && shoot_timer <= 0){
            pthread_create(&laser_thread, NULL, &music_routine, (void *)laser_sound) ;
            switch(diff){
                case 1:
                    shoot_timer = EASY_SHOT ;
                    break ;
                case 2:
                    shoot_timer = MEDIUM_SHOT ;
                    break ;
                case 3:
                    shoot_timer = HARD_SHOT ;
                    break ;
            }
            pthread_mutex_lock(&mutex) ;
            enemy_bullet_obj[id].y = log_obj[id].y+1 ;
            enemy_bullet_obj[id].x = log_obj[id].x+4 ;
            pthread_mutex_unlock(&mutex) ;
        }
        
        //la velocità di movimento nel tronco (e conseguente spawn dei nemici e sparo dei proiettili) cambia
        //in base alla difficoltà
        switch(log_obj[id].y){
            case 16:
                usleep(140000/diff) ;
                break ;
            case 13:
                usleep(260000/diff) ;
                break ;
            case 10:
                usleep(200000/diff) ;
                break ;
            case 7:
                usleep(120000/diff) ;
                break ;
        }
    }
}