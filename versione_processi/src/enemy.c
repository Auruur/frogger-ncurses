//inclusione dei file che contengono le librerie, le variabili globali e i prototipi di funzione 
#include "all.h"
#include "global.h"

void enemy(int p[2], int p_logtobullet[2], int (*p_ebullet)[2]){
    //dichiarazione degli oggetti 
    obj bullet ;
    obj sigbull ;
    sigbull.type = 'v' ;
    obj t ;
    read(p_logtobullet[0], &t, sizeof(obj)) ;

    //assegnazione della posizione di partenza del proiettile e del tipo 
    bullet.x = t.x+4 ;
    bullet.y = t.y+1 ;
    bullet.id = t.id ;
    int i = bullet.id-20 ;
    bullet.type = 'e' ;

    write(p[1], &bullet, sizeof(obj)) ;

    while(bullet.y <= maxy-1){
        //lettura del contenuto della pipe, la quale manda un segnale
        //se il proiettile ha avuto una collisione con qualche oggetto
        read(p_ebullet[i][0], &sigbull, sizeof(obj)) ;
        if(sigbull.type == 'e' && sigbull.id == bullet.id){
            //il proiettile viene fatto uscire dallo schermo e il processo viene killato
            sigbull.type = 'v' ;
            bullet.y = maxy-1 ;
        }
        bullet.y++ ;
        write(p[1], &bullet, sizeof(obj)) ;
        usleep(BULLET_SPEED) ;
    }
    _exit(0) ;
}