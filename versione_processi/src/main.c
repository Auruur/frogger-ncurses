#include "all.h"
#include "global.h"

int main(){
    //struttura che conterrà le statistiche di ritorno della partita
    endval result ;

    //flag che serve a rimanere nel ciclo di gioco e a fare più partite nella stessa esecuzione
    bool restart = TRUE ;

    //dichiarazione delle pipe
    int p[2] ;
    int p_frog[2] ;
    int p_car[2] ;
    int p_coda[2] ;
    int p_frogtobullet[2] ;
    int p_bullettofrog[2] ;
    int p_log[2] ;
    int p_time[2] ;
    int p_reset[2] ;
    int p_logtobullet[2] ;
    int p_bulletcollision[2] ;
    int p_onlog[NUM_T][2] ;
    int p_enemy[NUM_T][2] ;
    int p_ebullet[NUM_T][2] ;

    pipe(p) ;
    pipe(p_frog) ;
    pipe(p_car) ;
    pipe(p_coda) ;
    pipe(p_frogtobullet) ;
    pipe(p_bullettofrog) ;
    pipe(p_log) ;
    pipe(p_time) ;
    pipe(p_reset) ;
    pipe(p_logtobullet) ;
    pipe(p_bulletcollision) ;

    //alcune delle pipe vengono rese non bloccanti
    for(int i = 0 ; i < NUM_T ; i++){
        pipe(p_onlog[i]) ;
        pipe(p_enemy[i]) ;
        pipe(p_ebullet[i]) ;
        fcntl(p_onlog[i][0], F_SETFL, fcntl(p_onlog[i][0], F_GETFL) | O_NONBLOCK) ;        
        fcntl(p_enemy[i][0], F_SETFL, fcntl(p_enemy[i][0], F_GETFL) | O_NONBLOCK) ;
        fcntl(p_ebullet[i][0], F_SETFL, fcntl(p_ebullet[i][0], F_GETFL) | O_NONBLOCK) ;
    }

    fcntl(p_frog[0], F_SETFL, fcntl(p_frog[0], F_GETFL) | O_NONBLOCK) ;
    fcntl(p_frogtobullet[0], F_SETFL, fcntl(p_frogtobullet[0], F_GETFL) | O_NONBLOCK) ;
    fcntl(p_bullettofrog[0], F_SETFL, fcntl(p_bullettofrog[0], F_GETFL) | O_NONBLOCK) ;
    fcntl(p_time[0], F_SETFL, fcntl(p_time[0], F_GETFL) | O_NONBLOCK) ;
    fcntl(p_reset[0], F_SETFL, fcntl(p_reset[0], F_GETFL) | O_NONBLOCK) ;
    fcntl(p_log[0], F_SETFL, fcntl(p_log[0], F_GETFL) | O_NONBLOCK) ;
    fcntl(p_logtobullet[0], F_SETFL, fcntl(p_logtobullet[0], F_GETFL) | O_NONBLOCK) ;
    fcntl(p_bulletcollision[0], F_SETFL, fcntl(p_bulletcollision[0], F_GETFL) | O_NONBLOCK) ;

    //inizializzazione della libreria ncurses e configurazione parametri della window
    initscr() ;  
    noecho() ;     
    keypad(stdscr, 1) ; 
    curs_set(0) ;
    resize_term(38, 82) ;
    getmaxyx(stdscr, maxy, maxx) ;

    //la prima funzione attiva i colori della libreria
    //la seconda è creata da noi e inizializza i color pair che useremo in tutto il programma
    start_color() ;
    color_starter() ;
    
    //finchè restart è true il gioco riparte
    while(restart){
        //se la funzione menu ritorna TRUE il gioco parte, altrimenti viene chiuso
        if(menu()==FALSE){
            endwin() ;
            exit(0) ;
            return 0 ;
        }

        srand(time(NULL)) ;

        //inizializziamo in modo randomico il senso delle corsie e dei flussi
        initialize_road_river() ;

        //dichiarazione pid 
        pid_t pid_rana, pid_veicoli1, pid_veicoli2, pid_tronchi1, pid_tronchi2, pid_tronchi3, pid_tronchi4, pid_timer ;
        
        //segue una serie di fork atta a creare numerosi processi imparentati
        pid_rana = fork() ;

        if(pid_rana == 0) //rana
            rana(p, p_frog, p_frogtobullet, p_bullettofrog, p_bulletcollision, p_log) ;
        
        else if(pid_rana > 0)
            pid_veicoli1 = fork() ;

        if(pid_veicoli1 == 0){ //veicolo 1
            generatrice(p, p_car, p_coda) ;
        }

        else if(pid_veicoli1 > 0){
            pid_veicoli2 = fork() ;
        }
        
        if(pid_veicoli2 == 0){ //veicolo 2
            dati_veicolo(p_car, p_coda) ;
        }
        
        else if(pid_veicoli2 > 0){
            pid_tronchi1 = fork() ;
        }

        if(pid_tronchi1 == 0){
            tronco(p, p_onlog, p_enemy, p_logtobullet, p_ebullet, 20, f1) ;
        }

        else if(pid_tronchi1 > 0){
            pid_tronchi2 = fork() ;
        }

        if(pid_tronchi2 == 0){
            tronco(p, p_onlog, p_enemy, p_logtobullet, p_ebullet, 21, f2) ;
        }

        else if(pid_tronchi2 > 0){
            pid_tronchi3 = fork() ;
        }

        if(pid_tronchi3 == 0){
            tronco(p, p_onlog, p_enemy, p_logtobullet, p_ebullet, 22, f3) ; 
        }

        else if(pid_tronchi3 > 0){
            pid_tronchi4 = fork() ;
        }

        if(pid_tronchi4 == 0){
            tronco(p, p_onlog, p_enemy, p_logtobullet, p_ebullet, 23, f4) ;
        }

        else if(pid_tronchi4 > 0){
            pid_timer = fork() ;
        }
        
        if(pid_timer == 0){
            timer(p_time, p_reset) ;
        }
        
        else if(pid_timer > 0){
            //l'ultimo processo è il controllo che gestisce stampa e collisioni
            result = controllo(p, p_frog, p_log, p_onlog, p_enemy, p_bulletcollision, p_ebullet, p_time, p_reset) ;
        }

        //dopo il termine della partita vengono killati tutti i processi
        kill(pid_rana, 1) ;
        kill(pid_veicoli1, 1) ;
        kill(pid_veicoli2, 1) ;
        kill(pid_tronchi1, 1) ;
        kill(pid_tronchi2, 1) ;
        kill(pid_tronchi3, 1) ;
        kill(pid_tronchi4, 1) ;
        kill(pid_timer, 1) ;
        
        //in base alla vittoria/sconfitta del giocatore viene visualizzata una window diversa,
        //se all'interno di essa si preme spazio, la flag restart rimane TRUE e il gioco riparte
        if(result.result)
            restart = win(result.score) ;
        else if(!result.result)
            restart = gameover(result.score) ;
    }

    //blocco di uscita
    endwin() ;
    exit(1) ;
    return 0 ;
}

void initialize_road_river(){
    srand(time(NULL)) ;
    //corsie autostrada
    c1.y = 31 ;
    c2.y = 28 ;
    c3.y = 25 ;
    c4.y = 22 ;
    //corsie fiume
    f1.y = 16 ;
    f2.y = 13 ;
    f3.y = 10 ;
    f4.y = 7 ;

    switch(rand()%2){
        case 0:
            c1.senso = 1 ;
            break ;
        case 1:
            c1.senso = -1 ;
            break ;
    }

    switch(rand()%2){
        case 0:
            f1.senso = 1 ;
            break ;
        case 1:
            f1.senso = -1 ;
            break ;
    }

    c2.senso = c1.senso * -1 ;
    c3.senso = c2.senso * -1 ;
    c4.senso = c3.senso * -1 ;

    f2.senso = f1.senso * -1 ;
    f3.senso = f2.senso * -1 ;
    f4.senso = f3.senso * -1 ;
}

bool win(int score){
    clear() ;
    bkgd(COLOR_PAIR(6)) ;
    attron(COLOR_PAIR(13));

    mvprintw(16, 15, "  ") ;
    mvprintw(16, 19, "  ") ;    
    mvprintw(17, 15, "      ") ;
    mvprintw(18, 17, "  ") ;
    mvprintw(19, 17, "  ") ;
    mvprintw(20, 17, "  ") ;   

    mvprintw(16, 22, "      ") ;
    mvprintw(17, 22, "  ") ;
    mvprintw(17, 26, "  ") ;
    mvprintw(18, 26, "  ") ;
    mvprintw(18, 22, "  ") ;
    mvprintw(19, 22, "  ") ;
    mvprintw(19, 26, "  ") ;
    mvprintw(20, 22, "      ") ;

    mvprintw(16, 29, "  ") ;
    mvprintw(16, 33, "  ") ;
    mvprintw(17, 29, "  ") ;
    mvprintw(17, 33, "  ") ;
    mvprintw(18, 33, "  ") ;
    mvprintw(18, 29, "  ") ;
    mvprintw(19, 29, "  ") ;
    mvprintw(19, 33, "  ") ;
    mvprintw(20, 29, "      ") ;

    mvprintw(16, 42, "  ") ;
    mvprintw(16, 50, "  ") ;
    mvprintw(17, 42, "  ") ;
    mvprintw(17, 50, "  ") ;
    mvprintw(18, 42, "  ") ;
    mvprintw(18, 50, "  ") ;
    mvprintw(19, 42, "  ") ;
    mvprintw(19, 50, "  ") ;
    mvprintw(20, 42, "  ") ;
    mvprintw(19, 46, "  ") ;
    mvprintw(20, 44, "  ") ;
    mvprintw(20, 48, "  ") ;
    mvprintw(20, 50, "  ") ;

    mvprintw(16, 54, "  ") ;
    mvprintw(18, 54, "  ") ;
    mvprintw(19, 54, "  ") ;
    mvprintw(20, 54, "  ") ;

    mvprintw(16, 58, "  ") ;
    mvprintw(17, 58, "  ") ;
    mvprintw(18, 58, "  ") ;
    mvprintw(19, 58, "  ") ;
    mvprintw(20, 58, "  ") ;  
    mvprintw(16, 59, "  ") ;
    mvprintw(17, 60, "  ") ;
    mvprintw(18, 61, "  ") ;
    mvprintw(19, 62, "  ") ;
    mvprintw(16, 63, "  ") ;
    mvprintw(17, 63, "  ") ;
    mvprintw(18, 63, "  ") ;
    mvprintw(19, 63, "  ") ;
    mvprintw(20, 63, "  ") ;

    mvprintw(16, 67, "  ") ;
    mvprintw(17, 67, "  ") ;
    mvprintw(18, 67, "  ") ;
    mvprintw(20, 67, "  ") ;

    //stampa del punteggio 
    attron(COLOR_PAIR(6)) ;
    mvprintw(22, 35, "SCORE: %d", score) ;
    attron(A_BLINK) ;
    mvprintw(24, 27, "PRESS SPACEBAR TO PLAY AGAIN!") ;
    attroff(A_BLINK) ;
    attroff(COLOR_PAIR(6)) ;

    box(stdscr, ACS_VLINE, ACS_HLINE) ;

    refresh() ;

    timeout(-1) ;

    switch(getch()){
        case SPACE_BAR:
            return TRUE ;
            break ;
        default:
            return FALSE ;
            break ;
    }
}

bool gameover(int score){
    clear() ;
    bkgd(COLOR_PAIR(7)) ;
    attron(COLOR_PAIR(12));
    //LETTERA Y
    mvprintw(16, 13, "  ") ;
    mvprintw(16, 17, "  ") ;    
    mvprintw(17, 13, "      ") ;
    mvprintw(18, 15, "  ") ;
    mvprintw(19, 15, "  ") ;
    mvprintw(20, 15, "  ") ;   
    //LETTERA O
    mvprintw(16, 20, "      ") ;
    mvprintw(17, 20, "  ") ;
    mvprintw(17, 24, "  ") ;
    mvprintw(18, 24, "  ") ;
    mvprintw(18, 20, "  ") ;
    mvprintw(19, 20, "  ") ;
    mvprintw(19, 24, "  ") ;
    mvprintw(20, 20, "      ") ;
    //LETTERA U
    mvprintw(16, 27, "  ") ;
    mvprintw(16, 31, "  ") ;
    mvprintw(17, 27, "  ") ;
    mvprintw(17, 31, "  ") ;
    mvprintw(18, 31, "  ") ;
    mvprintw(18, 27, "  ") ;
    mvprintw(19, 27, "  ") ;
    mvprintw(19, 31, "  ") ;
    mvprintw(20, 27, "      ") ;
    //LETTERA L
    mvprintw(16, 40, "  ") ;
    mvprintw(17, 40, "  ") ;
    mvprintw(18, 40, "  ") ;
    mvprintw(19, 40, "  ") ;
    mvprintw(20, 40, "      ") ;
    //LETTERA O
    mvprintw(16, 47, "      ") ;
    mvprintw(17, 47, "  ") ;
    mvprintw(17, 51, "  ") ;
    mvprintw(18, 47, "  ") ;
    mvprintw(18, 51, "  ") ;
    mvprintw(19, 47, "  ") ;
    mvprintw(19, 51, "  ") ;
    mvprintw(20, 47, "      ") ;
    //LETTERA S
    mvprintw(16, 54, "      ") ;
    mvprintw(17, 54, "  ") ;
    mvprintw(18, 54, "      ") ;
    mvprintw(19, 58, "  ") ;
    mvprintw(20, 54, "      ") ;
    //LETTERA E
    mvprintw(16, 61, "      ") ;
    mvprintw(17, 61, "  ") ;
    mvprintw(18, 61, "      ") ;
    mvprintw(19, 61, "  ") ;
    mvprintw(20, 61, "      ") ;
    //PUNTO ESCLAMATIVO
    mvprintw(16, 69, "  ") ;
    mvprintw(17, 69, "  ") ;
    mvprintw(18, 69, "  ") ;
    mvprintw(20, 69, "  ") ;

    //stampa del punteggio 
    attron(COLOR_PAIR(7)) ;
    mvprintw(22, 35, "PUNTEGGIO: %d", score) ;
    attron(A_BLINK) ;
    mvprintw(24, 27, "PRESS SPACEBAR TO PLAY AGAIN!") ;
    attroff(A_BLINK) ;
    attroff(COLOR_PAIR(7)) ;

    box(stdscr, ACS_VLINE, ACS_HLINE) ;

    refresh() ;
    
    timeout(-1) ;

    switch(getch()){
        case SPACE_BAR:
            return TRUE ;
            break ;
        default:
            return FALSE ;
            break ;
    }
}

void color_starter(){
    init_pair(1, COLOR_WHITE, COLOR_BLUE) ;
    init_pair(2, 8, COLOR_WHITE) ;
    init_pair(3, COLOR_WHITE, COLOR_BLACK) ;
    init_pair(4, COLOR_WHITE, COLOR_CYAN) ;
    init_pair(5, COLOR_WHITE, COLOR_YELLOW) ;
    init_pair(6, COLOR_WHITE, COLOR_GREEN) ;
    init_pair(7, COLOR_WHITE, COLOR_RED) ;
    init_pair(8, COLOR_WHITE, COLOR_WHITE) ;
    init_pair(9, COLOR_GREEN, COLOR_GREEN) ;
    init_pair(10, COLOR_WHITE, COLOR_WHITE) ;
    init_pair(11, COLOR_GREEN, COLOR_GREEN) ;
    init_pair(12, COLOR_RED, COLOR_WHITE) ;
    init_pair(13, COLOR_GREEN, COLOR_WHITE) ;
    init_pair(20, COLOR_WHITE, COLOR_WHITE) ;
    init_pair(21, COLOR_WHITE, COLOR_BLACK) ;
    init_pair(22, COLOR_GREEN, COLOR_GREEN) ;
    init_pair(23, COLOR_WHITE, COLOR_BLUE) ;
    init_pair(24, COLOR_RED, COLOR_BLUE) ;
    init_pair(25, COLOR_YELLOW, COLOR_BLUE) ;
    init_pair(26, COLOR_GREEN, COLOR_BLUE) ;
    init_pair(27, COLOR_YELLOW, COLOR_YELLOW) ;
    init_pair(28, COLOR_RED, COLOR_RED) ;
}