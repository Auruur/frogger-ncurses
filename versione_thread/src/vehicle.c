#include "all.h"
#include "global.h"

void* vehicle_routine(void*k){
    //viene assegnato l'id passato nel main
    int id = *((int *) k) ;
    srand(id) ;
    int direzione ;
    vehicle_obj[id].id = id ;
    int corsie ;

    //randomicamente il veicolo è una macchina o un camion
    switch(rand()%2){
        case 0:
            vehicle_obj[id].type = 'm' ;
            break ;
        case 1:
            vehicle_obj[id].type = 'c' ;
            break ;
    }

    //al veicolo viene assegnata randomicamente una corsia,
    //solo se questa non è già occupata dal massimo di macchine definito dalla macro,
    //in quel caso reitera l'assegnazione
    bool flag1 = TRUE ;
    while(flag1){
        pthread_mutex_lock(&mutex) ;
        switch(rand()%4){
            case 0:
                if(traffico[0] < MAX_TRAFFIC){
                    traffico[0]++ ;
                    vehicle_obj[id].y = c1.y ;
                    direzione = c1.senso ;
                    flag1 = FALSE ;
                }
                break ;
            case 1:
                if(traffico[1] < MAX_TRAFFIC){
                    traffico[1]++ ;
                    vehicle_obj[id].y = c2.y ;
                    direzione = c2.senso ;
                    flag1 = FALSE ;
                }
                break ;
            case 2:
                if(traffico[2] < MAX_TRAFFIC){
                    traffico[2]++ ;
                    vehicle_obj[id].y = c3.y ;
                    direzione = c3.senso ;
                    flag1 = FALSE ;
                }
                break ;
            case 3:
                if(traffico[3] < MAX_TRAFFIC){
                    traffico[3]++ ;
                    vehicle_obj[id].y = c4.y ;
                    direzione = c4.senso ;
                    flag1 = FALSE ;
                }
                break ;   
        }
        pthread_mutex_unlock(&mutex) ;
    }

    //il veicolo viene spawnato casualmente lungo l'asse orizzontale (compreso nei bordi)
    vehicle_obj[id].x = rand()%35 ;

    bool flag2 = TRUE ;
    bool flag3 = TRUE ;

    //questo controllo assicura che la combinazione con cui vengono inizializzati tutti i veicoli non generi collisioni
    while(flag2){
        flag3 = true;
        for (int i = 0; i<NUM_VEHICLES; i++){
            for (int j = 0; j<NUM_VEHICLES; j++){
                if (vehicle_obj[i].y == vehicle_obj[j].y && i != j){
                    if (!(vehicle_obj[j].x > vehicle_obj[i].x+13 || vehicle_obj[j].x+13 < vehicle_obj[i].x))
                    {
                        pthread_mutex_lock(&mutex);
                        vehicle_obj[id].x=(1+rand()%35);
                        pthread_mutex_unlock(&mutex);
                        flag3 = false;
                    }
                }
            }
        }
        if (flag3){
            flag2 = false;
            //aumentiamo un semaforo per segnalare che il veicolo è stato inizializzato correttamente
            pthread_mutex_lock(&mutex) ;
            sem_post(&car_count) ;
            pthread_mutex_unlock(&mutex) ;
        }
    }

    //aumentiamo un semaforo per segnalare che il thread è stato creato e inizializzato correttamente
    pthread_mutex_lock(&mutex) ;
    sem_post(&count) ;
    pthread_mutex_unlock(&mutex) ;

    bool flag4 = TRUE ;
    bool flag5  = TRUE ;

    //in questo while viene gestito il movimento e il respawn nelle corsie
    while(true){
        //se il veicolo è compreso nei bordi si muove della sua direzione (+1/-1) e con la velocità in base alla difficoltà della partita
        if(((vehicle_obj[id].type == 'm' && vehicle_obj[id].x+6 > -2) || (vehicle_obj[id].type == 'c' && vehicle_obj[id].x+10 > -2)) && vehicle_obj[id].x < maxx+1){
            pthread_mutex_lock(&mutex) ;
            vehicle_obj[id].x += direzione ;
            pthread_mutex_unlock(&mutex) ;
            switch(diff){
                case 1:
                    usleep(EASY_CARSPEED) ;
                    break;
                case 2:
                    usleep(MEDIUM_CARSPEED) ;
                    break;
                case 3:
                    usleep(HARD_CARSPEED) ;
                    break ;
            }
        }
        //se il veicolo esce dai bordi il traffico della sua corsia viene decrementato
        else{
            switch(vehicle_obj[id].y){
                case 31:
                    traffico[0]-- ;
                    break ;
                case 28:
                    traffico[1]-- ;
                    break ;
                case 25:
                    traffico[2]-- ;
                    break ;
                case 22:
                    traffico[3]-- ;
                    break ;
            }
            //il tempo di respawn è parzialmente random per rendere la scena più originale e asincrona (tra i veicoli) 
            usleep(1000000+rand()%1000000) ;

            flag4 = TRUE ;
            flag5 = TRUE ;

            //casualmente viene riassegnata la corsia reiterando finchè non ne viene trovata una che non ha raggiunto
            //il numero massimo di veicoli
            while(flag4){
                pthread_mutex_lock(&mutex) ;
                while(flag5){
                    corsie = rand()%4 ;
                    switch(rand()%4){
                        case 0:
                            if(traffico[0] < MAX_TRAFFIC){
                                traffico[0]++ ;
                                vehicle_obj[id].y = c1.y ;
                                direzione = c1.senso ;
                                flag5 = FALSE ;
                            }
                            break ;
                        case 1:
                            if(traffico[1] < MAX_TRAFFIC){
                                traffico[1]++ ;
                                vehicle_obj[id].y = c2.y ;
                                direzione = c2.senso ;
                                flag5 = FALSE ;
                            }
                            break ;
                        case 2:
                            if(traffico[2] < MAX_TRAFFIC){
                                traffico[2]++ ;
                                vehicle_obj[id].y = c3.y ;
                                direzione = c3.senso ;
                                flag5 = FALSE ;
                            }
                            break ;
                        case 3:
                            if(traffico[3] < MAX_TRAFFIC){
                                traffico[3]++ ;
                                vehicle_obj[id].y = c4.y ;
                                direzione = c4.senso ;
                                flag5 = FALSE ;
                            }
                            break ;   
                    }
                }

                //la x del veicolo viene settata adeguatamente sul bordo in base al tipo (macchina/camion) e alla direzione di movimento
                if(direzione == 1)
                    vehicle_obj[id].x = 0 ;
                else if(direzione == -1){
                    if(vehicle_obj[id].type == 'm')
                        vehicle_obj[id].x = maxx-6 ;
                    else if(vehicle_obj[id].type == 'c')
                        vehicle_obj[id].x = maxx-10 ;
                }

                //se il veicolo collide al respawn viene rispawnato
                for(int i = 0 ; i < NUM_VEHICLES ; i++){
                    if(vehicle_obj[id].y == vehicle_obj[i].y && id != i && !(vehicle_obj[id].x+13 < vehicle_obj[i].x || vehicle_obj[id].x > vehicle_obj[i].x+13)){
                        flag4 = TRUE ;
                        break ;
                    }
                    else
                        flag4 = FALSE ;
                }
                pthread_mutex_unlock(&mutex) ;
            }
        }
    }    
}