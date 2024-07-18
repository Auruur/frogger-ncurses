#include "all.h"
#include "global.h"

void dati_veicolo(int p_car[2], int p_coda[2]){
    srand(time(NULL)) ;
    close(p_car[0]) ;
    //viene creata una coda di veicoli
    obj coda_veicoli[NUM_V] ;
    //viene creato un array che tiene conto del traffico
    int traffico[4] ;
    int i = 0 ;
    int corsie ;
    
    //l'array traffico viene inizializzato come vuoto
    for(int j = 0 ; j < 4 ; j++)
        traffico[j] = 0 ;
    
    //randomicamente la coda viene riempita con veicoli macchina o camion
    for(i = 0; i < NUM_V; i++){
        switch(rand()%2){
            case 0:
                coda_veicoli[i].type = 'm' ;
                break;
            case 1:
                coda_veicoli[i].type = 'c' ;
                break ;
        }
    }

    i = 0 ;

    while(true){
        //grazie all'operatore di modulo l'indicizzazione permette di scorrere in maniera 
        //circolare la coda

        //se quello slot della coda è diverso da 'v' (vuoto) viene assegnata una corsia random
        //finchè l'array del traffico non lo consente
        if(coda_veicoli[i%NUM_V].type != 'v'){
            do{
                corsie = rand()%4 ;
                switch(corsie){
                    case 0:
                        coda_veicoli[i%NUM_V].y = c1.y;
                        break;
                    case 1:
                        coda_veicoli[i%NUM_V].y = c2.y;
                        break;
                    case 2:
                        coda_veicoli[i%NUM_V].y = c3.y;
                        break;
                    case 3:
                        coda_veicoli[i%NUM_V].y = c4.y;
                        break;
                    }
            }while(traffico[corsie] >= MAX_TRAFFIC) ;
            //il veicolo viene spedito nella pipe generatrice
            write(p_car[1], &coda_veicoli[i%NUM_V], sizeof(obj)) ;
        }
        //il traffico della corsia scelta viene incrementato
        traffico[corsie]++ ;
        //lo slot della coda viene settato a vuoto
        coda_veicoli[i%NUM_V].type = 'v' ;
        //con una usleep parzialmente random si passa alla prossima iterazione (lo spawn avviene uno per volta e questo evita le collisioni)
        usleep(1200000 + rand()%700000) ;
        i++ ;

        //se lo slot della coda è vuoto viene riempito da una macchina uscita dalla generatrice e rispedita qua
        if(coda_veicoli[i%NUM_V].type == 'v'){
            read(p_coda[0], &coda_veicoli[i%NUM_V], sizeof(obj)) ;
            //il traffico della corsia della macchina appena uscita viene decrementato
            switch(coda_veicoli[i%NUM_V].y){
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
        }
    }
}

void generatrice(int p[2], int p_car[2], int p_coda[2]){
    close(p_car[1]) ;
    obj veicolo ;
    obj oldpos ;
    int direzione ;
    //viene inizializzato un array di pid
    pid_t pid_array[NUM_V] ;
    int i = 0 ;

    while(true){
        //viene letto da dati_veicolo il veicolo da generare
        read(p_car[0], &veicolo, sizeof(obj)) ;
        
        //gli viene assegnata una direzione in base alla corsia
        switch(veicolo.y){
            case 31:
                direzione = c1.senso;
                break ;
            case 28:
                direzione = c2.senso;
                break ;
            case 25:
                direzione = c3.senso;
                break ;
            case 22:
                direzione = c4.senso;
                break ;
        }

        //in base alla direzione e al tipo di veicolo viene spawnata adeguatamente al bordo della corsia
        if(direzione == 1)
            veicolo.x = 0 ;
        else if(direzione == -1){
            if(veicolo.type == 'm')
                veicolo.x = maxx-6 ;
            else if(veicolo.type == 'c')
                veicolo.x = maxx-10 ;
        }

        //viene fatta una fork sul pid del veicolo in considerazione
        pid_array[i%NUM_V] = fork() ;

        veicolo.id = i%NUM_V ;
        
        //il processo figlio gestisce il movimento (la velocità varia in base alla difficoltà)
        if(pid_array[i%NUM_V] == 0){
            //se il veicolo è compreso nei bordi
            while( ((veicolo.type == 'm' && veicolo.x+6 > -2) || (veicolo.type == 'c' && veicolo.x+10 > -2)) && veicolo.x < maxx+1){
                //si muove della sua direzione
                veicolo.x += direzione ;
                //viene scritto nella pipe principale per essere stampato solo se è cambiata la posizione (per non intasarla)
                if(oldpos.x != veicolo.x)
                    write(p[1], &veicolo, sizeof(obj)) ;
                oldpos = veicolo ;
                switch(diff){
                    case 1:
                        usleep(EASY_CARSPEED) ;
                        break ;
                    case 2:
                        usleep(MEDIUM_CARSPEED) ;
                        break ;
                    case 3:
                        usleep(HARD_CARSPEED) ;
                        break ;
                }
            }
            //quando il veicolo esce dal bordo viene rispedito nella coda e il processo killato
            write(p_coda[1], &veicolo, sizeof(obj)) ;
            _exit(0) ;
        }

        i++ ;
    }
}