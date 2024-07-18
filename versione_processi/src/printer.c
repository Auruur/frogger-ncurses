//inclusione dei file che contengono le librerie, le variabili globali e i prototipi di funzione 
#include "all.h"
#include "global.h"

void drawmap(){
    getmaxyx(stdscr, maxy, maxx) ;

    //COLORE BASE
    attron(COLOR_PAIR(5)) ;
    for(int i = 0; i < 5 ; i++)
        mvhline(i, 0, ' ', maxx) ;

    //COLORE FIUME
    attron(COLOR_PAIR(4)) ;
    for(int i = 5; i < 17 ; i++)
        mvhline(i, 0, ' ', maxx) ;
    
    //COLORE ERBA
    attron(COLOR_PAIR(2)) ;
    for(int i = 17; i < 20; i++) 
        mvhline(i, 0, '-', maxx) ;

    //COLORE STRADA
    attron(COLOR_PAIR(3)) ;
    for(int i = 20; i < 32; i++)
        mvhline(i, 0, ' ', maxx) ;

    //COLORE MARCIAPIEDE
    attron(COLOR_PAIR(2)) ;
    for(int i = 32; i < 35; i++)
        mvhline(i, 0, '-', maxx) ;

    //COLORE STATISTICHE
    attron(COLOR_PAIR(1)) ;
    mvhline(35, 0, ' ', maxx) ;
    
    //COLORE DELLE SINGOLE BASI 
    //se le basi non sono state raggiunte dalla rana vengono visualizzate come 'vuote'
    //una volta che la rana raggiunge una delle basi questa viene riempita e visualizzata come 'piena'
    //COLORE BASE 1
    attron(COLOR_PAIR(4)) ;
    for(int i = 2; i < 12; i++){
        if(b1_flag){
            attron(COLOR_PAIR(4)) ;
            mvvline(2, i, ' ', 3) ;
        }
        else{
            attron(COLOR_PAIR(5)) ;
            mvvline(2, i, '=', 3) ;
        }
    }
    
    
    //COLORE BASE 2
    for(int i=19; i < 29; i++){
        if(b2_flag){
            attron(COLOR_PAIR(4)) ;
            mvvline(2, i, ' ', 3) ;
        }
        else{
            attron(COLOR_PAIR(5)) ;
            mvvline(2, i, '=', 3) ;
        }
    }

    //COLORE BASE 3
    for(int i=36; i < 46; i++){
        if(b3_flag){
            attron(COLOR_PAIR(4)) ;
            mvvline(2, i, ' ', 3) ;
        }
        else{
            attron(COLOR_PAIR(5)) ;
            mvvline(2, i, '=', 3) ;
        }
    }

    //COLORE BASE 4
    for(int i=53; i < 63; i++){
        if(b4_flag){
            attron(COLOR_PAIR(4)) ;
            mvvline(2, i, ' ', 3) ;
        }
        else{
            attron(COLOR_PAIR(5)) ;
            mvvline(2, i, '=', 3) ;
        }
    }
    
    //COLORE BASE 5
    for(int i=70; i < 80; i++){
        if(b5_flag){
            attron(COLOR_PAIR(4)) ;
            mvvline(2, i, ' ', 3) ;
        }
        else{
            attron(COLOR_PAIR(5)) ;
            mvvline(2, i, '=', 3) ;
        }
    }

    attroff(COLOR_PAIR(4)) ;
}