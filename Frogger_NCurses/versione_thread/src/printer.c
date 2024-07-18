#include "all.h"
#include "global.h"

void printer(){
    erase() ;

    drawmap() ;
    
    //*****************************STAMPA STATISTICHE************************************************************************

    //stampa vite
    attron(COLOR_PAIR(1)) ;
    mvaddch(35, 1, '|') ;
    mvaddch(36, 1, '|') ;
    mvprintw(35, 2, "VITE:") ;
    attroff(COLOR_PAIR(1)) ;
    attron(A_BOLD) ;
    switch(life){
        case 1:
            attron(A_BLINK) ;
            attron(COLOR_PAIR(24)) ;
            mvprintw(36, 2, "<3") ;
            attroff(COLOR_PAIR(24)) ;
            attroff(A_BLINK) ;
            break ;
        case 2:
            attron(COLOR_PAIR(25)) ;
            mvprintw(36, 2, "<3<3") ;
            attroff(COLOR_PAIR(25)) ;
            break ;
        case 3:
            attron(COLOR_PAIR(26)) ;
            mvprintw(36, 2, "<3<3<3") ;
            attroff(COLOR_PAIR(26)) ;
            break ;
        case 4:
            attron(COLOR_PAIR(26)) ;
            mvprintw(36, 2, "<3<3<3<3") ;
            attroff(COLOR_PAIR(26)) ;
            break ;
        case 5:
            attron(COLOR_PAIR(26)) ;
            mvprintw(36, 2, "<3<3<3<3<3") ;
            attroff(COLOR_PAIR(26)) ;
            break ;
    }
    attroff(A_BOLD) ;

    mvaddch(35, 15, '|') ;
    mvaddch(36, 15, '|') ;

    //stampa punteggio
    mvprintw(35, 16, "PUNTEGGIO:") ;
    attron(A_BOLD) ;
    mvprintw(36, 16, "%d", score) ;
    attroff(A_BOLD) ;

    mvaddch(35, 30, '|') ;
    mvaddch(36, 30, '|') ;

    //stampa basi
    mvprintw(35, 31, "BASI RIMANENTI:") ;
    attron(A_BOLD) ;
    mvprintw(36, 31, "%d", lairs_left) ;
    attroff(A_BOLD) ;

    mvaddch(35, 50, '|') ;
    mvaddch(36, 50, '|') ;

    //stampa tempo
    mvprintw(35, 51, "TEMPO RIMANENTE:") ;
    // mvprintw(36, 53, "%d", time_left) ;
    if(time_left >= 20){
        attron(COLOR_PAIR(11)) ;
        mvhline(36, 51, ' ', time_left) ;
        attroff(COLOR_PAIR(11)) ;
    }
    else if(time_left < 20 && time_left >= 10){
        attron(COLOR_PAIR(27)) ;
        mvhline(36, 51, ' ', time_left) ;
        attroff(COLOR_PAIR(27)) ;
    }
    else if(time_left < 10){
        attron(A_BLINK) ;
        attron(COLOR_PAIR(7)) ;
        mvhline(36, 51, '!', time_left) ;
        attroff(COLOR_PAIR(7)) ;
        attroff(A_BLINK) ;
    }
    attroff(COLOR_PAIR(1)) ;

    //*****************************FINE STAMPA STATISTICHE************************************************************************
    
    //*****************************STAMPA VEICOLI************************************************************************
    //i veicoli vengono stampati in modo tale che uscendo dai bordi spariscano gradualmente,
    //questo viene fatto stampando stringhe parziali dopo aver verificato quale parte del veicolo è compresa e quale no
    for(int i = 0 ; i < NUM_VEHICLES ; i++){
        attron(COLOR_PAIR(21)) ;
        //MACCHINA
        if(vehicle_obj[i].type == 'm'){
            //STA USCENDO A DESTRA
            if(vehicle_obj[i].x+6 >= maxx){
                for(int j = 0 ; j < 6; j++){
                    if(vehicle_obj[i].x+j <= maxx){
                        mvaddch(vehicle_obj[i].y-2, vehicle_obj[i].x+j, car_sprite_h[j]) ;
                        mvaddch(vehicle_obj[i].y-1, vehicle_obj[i].x+j, car_sprite_m[j]) ;
                        mvaddch(vehicle_obj[i].y, vehicle_obj[i].x+j, car_sprite_l[j]) ;
                    }
                }
            }
            //STA USCENDO A SINISTRA
            if(vehicle_obj[i].x < 0){
                for(int j = 0 ; j < 6 ; j++){
                    if(vehicle_obj[i].x+j >= 0){
                        mvaddch(vehicle_obj[i].y-2, vehicle_obj[i].x+j, car_sprite_h[j]) ;
                        mvaddch(vehicle_obj[i].y-1, vehicle_obj[i].x+j, car_sprite_m[j]) ;
                        mvaddch(vehicle_obj[i].y, vehicle_obj[i].x+j, car_sprite_l[j]) ;
                    }
                }
            }
            //E' COMPRESO
            else if(vehicle_obj[i].x >= 0 && vehicle_obj[i].x+6 <= maxx-1){
                mvprintw(vehicle_obj[i].y-2, vehicle_obj[i].x, " ____ ") ;
                mvprintw(vehicle_obj[i].y-1, vehicle_obj[i].x, "_|xx|_") ;
                mvprintw(vehicle_obj[i].y, vehicle_obj[i].x, ":o--o:") ;
            }
        }
        //CAMION
        else if(vehicle_obj[i].type == 'c'){
            //STA USCENDO A DESTRA
            if(vehicle_obj[i].x+10 >= maxx){
                for(int j = 0 ; j < 10; j++){
                    if(vehicle_obj[i].x+j <= maxx){
                        mvaddch(vehicle_obj[i].y-2, vehicle_obj[i].x+j, cam_sprite_h[j]) ;
                        mvaddch(vehicle_obj[i].y-1, vehicle_obj[i].x+j, cam_sprite_m[j]) ;
                        mvaddch(vehicle_obj[i].y, vehicle_obj[i].x+j, cam_sprite_l[j]) ;
                    }
                }
            }
            //STA USCENDO A SINISTRA
            if(vehicle_obj[i].x < 0){
                for(int j = 0 ; j < 10 ; j++){
                    if(vehicle_obj[i].x+j >= 0){
                        mvaddch(vehicle_obj[i].y-2, vehicle_obj[i].x+j, cam_sprite_h[j]) ;
                        mvaddch(vehicle_obj[i].y-1, vehicle_obj[i].x+j, cam_sprite_m[j]) ;
                        mvaddch(vehicle_obj[i].y, vehicle_obj[i].x+j, cam_sprite_l[j]) ;
                    }
                }
            }
            //E' COMPRESO
            else if(vehicle_obj[i].x >= 0 && vehicle_obj[i].x+10 <=maxx-1){
                mvprintw(vehicle_obj[i].y-2, vehicle_obj[i].x, "__________") ;
                mvprintw(vehicle_obj[i].y-1, vehicle_obj[i].x, "| x xx x |") ;
                mvprintw(vehicle_obj[i].y, vehicle_obj[i].x, ":-0----0-:") ;
            }
        }
        attroff(COLOR_PAIR(21)) ;
    }
    //*****************************FINE STAMPA VEICOLI************************************************************************

    //*****************************ALTRE STAMPE************************************************************************

    //stampa tronco
    attron(COLOR_PAIR(21)) ;
    for(int i = 0 ; i < NUM_LANES ; i++){
        mvprintw(log_obj[i].y-2, log_obj[i].x, "|========|") ;
        mvprintw(log_obj[i].y-1, log_obj[i].x, "|========|") ;
        mvprintw(log_obj[i].y, log_obj[i].x, "|========|") ;
    }
    attroff(COLOR_PAIR(21)) ;

    //stampa nemico
    for(int i = 0 ; i < NUM_LANES ; i++){
        if(log_obj[i].enemy == TRUE){
            attron(COLOR_PAIR(7)) ;
            mvprintw(log_obj[i].y-2, log_obj[i].x+3, "|\\/|") ;
            mvprintw(log_obj[i].y-1, log_obj[i].x+3, "|xx|") ;
            mvprintw(log_obj[i].y, log_obj[i].x+3, "(<>)") ;
            attroff(COLOR_PAIR(7)) ;
        }
    }
    
    //stampa proiettile nemico
    for(int i = 0 ; i < NUM_LANES ; i++){
        if(enemy_bullet_obj[i].y <= maxx-1){
            attron(COLOR_PAIR(7)) ;
            mvaddch(enemy_bullet_obj[i].y, enemy_bullet_obj[i].x, 'v') ;
            attroff(COLOR_PAIR(7)) ;
        }
    }   

    //stampa rana
    attron(COLOR_PAIR(6)) ;
    mvprintw(frog_obj.y-2, frog_obj.x, "|@ @|") ;
    mvprintw(frog_obj.y-1, frog_obj.x, "(---)") ;
    mvprintw(frog_obj.y, frog_obj.x, "<(X)>") ;
    attroff(COLOR_PAIR(6)) ;

    //stampa proiettile
    if(bullet_obj.y >= 0){
        attron(COLOR_PAIR(7)) ;
        mvaddch(bullet_obj.y, bullet_obj.x, 'o') ;
        attroff(COLOR_PAIR(7)) ;
    }    
    //*****************************FINE ALTRE STAMPE************************************************************************

    //la finestra è racchiusa dalla box per poterla distinguere facilmente anche col terminale in fullscreen
    box(stdscr, ACS_VLINE, ACS_HLINE) ;
    
    refresh() ;
}

void drawmap(){

    //COLORE BASE
    attron(COLOR_PAIR(5)) ;
    for(int i = 0; i < 5 ; i++)
        mvhline(i, 0, ' ', maxx) ;
    attroff(COLOR_PAIR(5)) ;

    //COLORE FIUME
    attron(COLOR_PAIR(4)) ;
    for(int i = 5; i < 17 ; i++)
        mvhline(i, 0, ' ', maxx) ;
    attroff(COLOR_PAIR(4)) ;
    
    //COLORE ERBA
    attron(COLOR_PAIR(2)) ;
    for(int i = 17; i < 20; i++) 
        mvhline(i, 0, '-', maxx) ;
    attroff(COLOR_PAIR(2)) ;

    //COLORE STRADA
    attron(COLOR_PAIR(3)) ;
    for(int i = 20; i < 32; i++)
        mvhline(i, 0, ' ', maxx) ;
    attroff(COLOR_PAIR(3)) ;

    //COLORE MARCIAPIEDE
    attron(COLOR_PAIR(2)) ;
    for(int i = 32; i < 35; i++)
        mvhline(i, 0, '-', maxx) ;
    attroff(COLOR_PAIR(2)) ;

    //COLORE STATISTICHE
    attron(COLOR_PAIR(1)) ;
    mvhline(35, 0, ' ', maxx) ;
    attroff(COLOR_PAIR(1)) ;
    
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