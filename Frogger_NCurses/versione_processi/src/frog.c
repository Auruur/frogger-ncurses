//inclusione dei file che contengono le librerie, le variabili globali e i prototipi di funzione 
#include "all.h"
#include "global.h"

void rana(int p[2], int p_frog[2], int p_frogtobullet[2], int p_bullettofrog[2], int p_bulletcollision[2], int p_log[2]){
    getmaxyx(stdscr, maxy, maxx) ;
    //dichiarazione degli oggetti e asegnazione dei valori di partenza all'oggetto 'rana'
    obj r = {maxy-4, maxx/2, '#', 'r', 300, FALSE, FALSE} ;
    obj oldpos ;
    oldpos = r ;
    bool collision = FALSE ;
    obj letto ;
    letto.t_flag = FALSE ;
    pid_t pid_bullet ;  //pid per la creazione del processo 'proiettile'
    write(p[1], &r, sizeof(obj)) ;

    while(true){
        getmaxyx(stdscr, maxy, maxx) ;  

        read(p_frog[0], &collision, sizeof(collision)) ;  
        read(p_bullettofrog[0], &r.flag, sizeof(bool)) ;
        read(p_log[0], &letto, sizeof(obj)) ;

        //lettura della variabile collisione dalla funzione controllo 
        //se si hanno collisioni la rana torna nella posizione di partenza
        if(collision == TRUE){
            collision = FALSE ;
            r.y = maxy-4;
            r.x = maxx/2 ;
            write(p[1], &r, sizeof(obj)) ;
        }

        //la pipe 'p_log' verifica che la rana sia sul tronco
        //in tal caso la rana assume le posizioni che vengono mandate dalla pipe
        if(letto.t_flag == TRUE){
            r.x = letto.x ;
            r.y = letto.y ;
            letto.t_flag = FALSE ;
        }
        
        //gestione delle collisioni con le basi 
        //se la rana raggiunge le basi torna alla posizione di partenza 
        if(r.y == 4 && ((r.x >= 2 && r.x+5 <= 12) || (r.x >= 19 && r.x+5<=29) || (r.x >= 36 && r.x+5 <= 46) || (r.x >= 53 && r.x+5 <= 63) || (r.x >= 70 && r.x+5 <= 80))){
            r.y = maxy-4 ;
            r.x = maxx/2 ;
            write(p[1], &r, sizeof(obj)) ;
        }
        
        timeout(1) ;

        //gestione del movimento della rana 
        int s = getch() ;
        switch(s){
            case KEY_UP:
                if(r.y > 5)
                    r.y -= 3;
                break;
            case KEY_DOWN:
                if(r.y < maxy-4)
                    r.y += 3;
                break;
            case KEY_LEFT:
                if(r.x > 0){
                    r.x -= 1;
                }
                break;
            case KEY_RIGHT:   
                if(r.x+4 < maxx-1){
                    r.x += 1;
                }
                break;
            case SPACE_BAR://SPACEBAR
            //creazione del processo 'proiettile' se non ci sono altri processi figli attivi 
            //WNOHANG rende il waitpid non bloccante
            if(waitpid(-1, NULL, WNOHANG) != 0){
                write(p_frogtobullet[1], &r, sizeof(obj)) ;
                pid_bullet = fork() ;
                if(pid_bullet == 0)
                    proiettile(p, p_frogtobullet, p_bulletcollision) ;
                break ;
            }
        }
    
        if(r.x != oldpos.x || r.y != oldpos.y)
            write(p[1], &r, sizeof(obj)) ;
        oldpos = r ;
    }
}
