#include "all.h"
#include "global.h"

int main(){
    //file per il debug
    fp = fopen("log.txt", "w") ;

    //inizializziamo la libreria e i parametri dello schermo
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

    while(restart){
        srand(time(NULL)) ;
        //se la funzione menu ritorna TRUE il gioco parte, altrimenti viene chiuso
        if(menu() == FALSE){
            endwin() ;
            exit(0) ;
            return 0 ;
        }

        //questi due contatori servono a tenere traccia dei thread attivi, 
        //per evitare di stampare/controllare collisioni prima che tutti i thread siano stati correttamente creati
        sem_init(&count, 0, 0) ;
        sem_init(&car_count, 0, 0) ;

        //vengono inizializzate le vite in base alla difficoltà
        switch(diff){
        case 1:
            life = EASY_LIFE ;
            break ;
        case 2:
            life = MEDIUM_LIFE ;
            break; 
        case 3:
            life = HARD_LIFE ;
            break ;
        }
        
        //inizializziamo l'array che gestisce il traffico a 0
        for(int i = 0 ; i < NUM_LANES ; i++)
            traffico[i] = 0 ;
        
        //mettiamo in una posizione fittizia l'oggetto proiettile
        pthread_mutex_lock(&mutex) ;
        bullet_obj.x = -1 ;
        bullet_obj.y = -1 ;
        pthread_mutex_unlock(&mutex) ;

        //mettiamo in una posizione fittizia gli oggetti proiettili dei nemici
        for(int i = 0 ; i < NUM_LANES ; i++){
            pthread_mutex_lock(&mutex) ;
            enemy_bullet_obj[i].x = i ;
            enemy_bullet_obj[i].y = maxx ;
            pthread_mutex_unlock(&mutex) ;
        }
        
        //inizializziamo in modo randomico il senso delle corsie e dei flussi
        initialize_road_river() ;

        //dichiariamo i thread
        pthread_t frog_thread ;
        pthread_t bullet_thread ;
        pthread_t vehicle_thread[NUM_VEHICLES] ;
        pthread_t log_thread[NUM_LANES] ;
        pthread_t enemy_bullet_thread[NUM_LANES] ;
        pthread_t timer_thread ;    

        //creaiamo degli id da passare ai thread per poterli riconoscere
        int car_id[NUM_VEHICLES] ;
        for(int i = 0 ; i < NUM_VEHICLES ; i++)
            car_id[i] = i ;

        int log_id[NUM_LANES] ;
        for(int i = 0 ; i < NUM_LANES ; i++)
            log_id[i] = i ;

        //creaiamo i thread
        pthread_create(&frog_thread, NULL, &frog_routine, NULL) ;
        pthread_create(&bullet_thread, NULL, &bullet_routine, NULL) ;

        for(int i=0 ; i < NUM_VEHICLES ; i++)
            pthread_create(&vehicle_thread[i], NULL, &vehicle_routine, (void*)&car_id[i]) ;

        for(int i=0 ; i < NUM_LANES ; i++){
            pthread_create(&log_thread[i], NULL, &log_routine, (void*)&log_id[i]) ;
            pthread_create(&enemy_bullet_thread[i], NULL, &enemy_routine, (void*)&log_id[i]) ;
        }

        pthread_create(&timer_thread, NULL, &timer_routine, NULL) ;

        //avviamo la zona di controllo che tiene traccia di collisioni e win/game condition
        bool result ;
        result = control_zone() ;

        //cancelliamo i thread
        pthread_cancel(frog_thread) ;
        pthread_cancel(bullet_thread) ;
        pthread_cancel(timer_thread) ;
        for(int i=0 ; i < NUM_VEHICLES ; i++)
            pthread_cancel(vehicle_thread[i]) ;
        for(int i=0 ; i < NUM_LANES ; i++){
            pthread_cancel(log_thread[i]) ;
            pthread_cancel(enemy_bullet_thread[i]) ;
        }
        //cancelliamo i semafori
        sem_destroy(&count) ;
        sem_destroy(&car_count) ;
        pthread_mutex_destroy(&mutex) ;

        //in base al booleano di uscita del controllo facciamo una stampa di vittoria/sconfitta
        if(result)
            win() ;
        else if(!result)
            gameover() ;
    }
    //blocco di uscita dal programma
    fclose(fp) ;
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

void win(){
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
            restart = TRUE ;
            break ;
        default:
            restart = FALSE ;
            break ;
    }

    return ;
}

void gameover(){
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
            restart = TRUE ;
            break ;
        default:
            restart = FALSE ;
            break ;
    }

    return ;
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