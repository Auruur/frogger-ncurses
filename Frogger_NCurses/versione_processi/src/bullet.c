#include "all.h"
#include "global.h"

void proiettile(int p[2], int p_frogtobullet[2], int p_bulletcollision[2]){
    obj bullet ;
    obj sigbull ;
    //'v' sta a indicare vuoto
    sigbull.type = 'v' ;
    obj r ;
    pid_t pid_sound ;
    //path del suono da riprodurre quando la rana spara
    char frog_sound[] = "src/sounds/frogsound.wav" ;
    //viene creato un processo per riprodurre il suono
    pid_sound = fork() ;
    if(pid_sound == 0)
        music(frog_sound) ;
    //il padre si occupa di muovere il proiettile
    else{
        //legge dalla rana la sua posizione
        read(p_frogtobullet[0], &r, sizeof(obj)) ;

        //il proiettile viene messo sopra la rana
        bullet.x = r.x+2 ;
        bullet.y = r.y-3 ;
        bullet.type = 'b' ;
        bullet.id = 500 ;

        //viene scritto inizialmente nella pipe per stampare la prima posizone
        write(p[1], &bullet, sizeof(obj)) ;

        //finchè è compreso nello schermo viene fatto muovere
        while(bullet.y >= 0){
            //ogni ciclo legge dalla pipe non bloccante, se il proiettile ha fatto una collisione
            //viene messo fuori dallo schermo e di conseguenza esce dal while
            read(p_bulletcollision[0], &sigbull, sizeof(obj)) ;
            if(sigbull.type == 'b'){
                sigbull.type = 'v' ;
                bullet.y = 0 ;
            }
            bullet.y-- ;
            write(p[1], &bullet, sizeof(obj)) ;
            usleep(BULLET_SPEED) ;
        }
        //quando il proiettile esce dal while il processo viene killato
        _exit(0) ;
    }
}