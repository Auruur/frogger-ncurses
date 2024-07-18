#include "all.h"
#include "global.h"

//inclusione dei file che contengono le librerie, le variabili globali e i prototipi di funzione 
void tronco(int p[2], int(*p_onlog)[2], int (*p_enemy)[2], int p_logtobullet[2], int (*p_ebullet)[2], int id, corsia fiume){
    srand(getpid()) ;
    getmaxyx(stdscr, maxy, maxx) ;

    //dichiarazione degli oggetti 
    obj t ;
    obj oldpos ;
    obj letto ;
    obj sigfrog ;

    //assegnazione dei valori di partenza all'oggetto tronco 
    sigfrog.t_flag = FALSE ;
    t.y = fiume.y ;
    t.id = id ;
    int i = t.id-20 ;
    t.x = maxx/2 - 5 ;
    t.flag = FALSE ;
    t.type = 't' ;
    t.enemy = FALSE ;
    oldpos = t ;
    pid_t bullet_pid ;  //pid per la creazione del proiettile
    int enemy_timer = 120+rand()%300 ; //timer per la frequenza di spawn del nemico 
    int shoot_timer ;
    //varie temporizzzazioni di sparo in base alla difficoltà 
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

    write(p[1], &t, sizeof(obj)) ;

    while(true){
        read(p_enemy[i][0], &letto, sizeof(obj)) ;
        read(p_onlog[i][0], &sigfrog, sizeof(obj)) ;

        //assegnazione del contenuto della pipe
        if(t.enemy == TRUE && letto.type == 'b' && letto.y >= t.y-2 && letto.y <= t.y){
            letto.type = 'v' ;
            t.enemy = FALSE ;
        }

        getmaxyx(stdscr, maxy, maxx) ;
        if(t.x != oldpos.x)
            write(p[1], &t, sizeof(obj)) ;
    
        oldpos = t ;

        //gestione delle collisioni dei tronchi con i bordi 
        if(t.x == 0 || t.x+9 == maxx-1)
            fiume.senso *= -1 ;

        //gestione dei contatori per la frequenza di spawn e sparo dei nemici
        if(t.enemy == FALSE)
            enemy_timer-- ;

        if(t.enemy == TRUE){
            shoot_timer-- ;
        }

        //creazione del processo 'enemy' se non ci sono altri processi figli attivi e se 
        //il contatore arriva a 0. Viene anche assegnata la velocita' di sparo. 
        // WNOHANG rende il waitpid non bloccante 
        if(waitpid(-1, NULL, WNOHANG) != 0 && shoot_timer <= 0){
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
            write(p_logtobullet[1], &t, sizeof(obj)) ;
            bullet_pid = fork() ;
            if(bullet_pid == 0){
                enemy(p, p_logtobullet, p_ebullet) ;
            }
        }   

        //se la rana non e' sul tronco viene inizializzato il contatore per lo sparo dei proiettili nemici
        if(enemy_timer <= 0 && sigfrog.t_flag != TRUE){
            enemy_timer = 120 + rand()%1000 ;
            t.enemy = TRUE ;
        }

        sigfrog.t_flag = FALSE ;

        //movimento del tronco 
        t.x += fiume.senso ;

        //gestione delle diverse velocita' dei tronchi in base alla difficoltà
        switch(t.y){
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
