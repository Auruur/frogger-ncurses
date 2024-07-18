#include "all.h"
#include "global.h"

endval controllo(int p[2], int p_frog[2], int p_log[2], int (*p_onlog)[2], int (*p_enemy)[2], int p_bulletcollision[2], int (*p_ebullet)[2], int p_time[2], int p_reset[2]){
    //dichiariamo e inizializziamo gli oggetti di supporto per la stampa in cui copiamo la lettura delle pipe
    obj letto ;
    obj rana ;
    obj oldfrog ;
    rana.y = maxy-4 ;
    rana.x = maxx/2 ;
    rana.id = 300 ;
    obj bullet ;
    bullet.x = -1 ;
    bullet.y = -1 ;
    bullet.id = 500 ;
    obj veicolo[NUM_V] ;
    obj t[NUM_T] ;
    obj old_t[NUM_T] ;
    obj b_enemy[NUM_T] ;

    //inizializziamo i due processi per riprodurre il suono di quando la base viene riempita e quando la rana perde una vita
    pid_t pid_deathsound ;
    pid_t pid_pointsound ;
    //path dei due suoni
    char death_sound[] = "src/sounds/death.wav" ;
    char point_sound[] = "src/sounds/win.wav" ;

    //diamo ai proiettili nemici una posizione iniziale fittizia
    for(int i = 0 ; i < NUM_T ;i++)
        b_enemy[i].y = -2 ;

    //inizializziamo le vite in base alla difficolta
    int vite ;
    switch(diff){
        case 1:
            vite = EASY_LIFE ;
            break ;
        case 2:
            vite = MEDIUM_LIFE ;
            break ;
        case 3:
            vite = HARD_LIFE ;
            break ;
    }
    //inizializziamo le statistiche
    int basi = NUM_LAIRS ;
    int time_left = TIME_LEFT ;
    int score = 0 ;

    //inizializziamo alcune flag che serviranno nel controllo
    bool exit = FALSE ;
    bool win_flag = FALSE ;
    bool gameover_flag = FALSE ;
    bool collision = FALSE ;
    bool log = FALSE ;
    bool hit = TRUE ;
    bool reset_time = TRUE ;

    getmaxyx(stdscr, maxy, maxx) ;
    
    while(true){
        //a ogni ciclo leggiamo dalla pipe e salviamo nella struttura letto
        read(p[0], &letto, sizeof(obj)) ;
        //questa pipe legge semplicemente il tempo dal processo timer
        read(p_time[0], &time_left, sizeof(int)) ;

        //*****************************LETTURA DELLA PIPE************************************************************************
        //copiamo in base al tipo e in alcuni casi all'id, il contenuto di letto dentro gli oggetti di supporto alla stampa
        if(letto.type == 'r')
            rana = letto ;
        for(int i = 0 ; i < NUM_V ; i++){
            if(letto.id == i){
                veicolo[i] = letto ;
            }
        }
        if(letto.type == 'b'){
            bullet = letto ;
        }
        for(int i = 20; i < 24 ; i++){
            if(letto.id == i && letto.type == 't')
                t[i-20] = letto ;
        }
        for(int i = 20 ; i < 24 ; i++){
            if(letto.id == i && letto.type == 'e')
                b_enemy[i-20] = letto ;
        }
        //*****************************FINE LETTURA DELLA PIPE************************************************************************

        //questo controllo gestisce il respawn dalla rana e della sua flag in seguito a una collisone,
        //se è nello spawn significa che è respawnata e possiamo risettare a FALSE la flag
        if(rana.x == maxx/2 && rana.y == maxy- 4){
            collision = FALSE ;
        }

        //*****************************RANA SUI TRONCHI************************************************************************

        //se la vecchia posizione della rana è sul tronco e quella nuova è sull'erba significa che è scesa
        if(oldfrog.y==t[0].y && rana.y==t[0].y+3){
            rana.t_flag = TRUE ;
            write(p_log[1], &rana, sizeof(obj)) ;
            rana.t_flag = FALSE ;
            oldfrog.y = -1 ; 
        }

        //se la rana è compresa nelle corsie del fiume
        if(rana.y<=t[0].y && rana.y>=t[3].y){
            //viene settato il flag a TRUE
            rana.t_flag =  TRUE ;
            for(int i = 0; i<4; i++){
                //se il tronco si muove e la rana è sul tronco
                if(old_t[i].x != t[i].x && rana.y == t[i].y){
                    //la rana si muove della suo posizione più l'offset rispetto al tronco
                    rana.x=rana.x+(t[i].x-old_t[i].x) ;
                    //se la rana è nella corsia del tronco ma non nel tronco muore (un processo riproduce il suono)
                    //il timer viene resettato
                    if(rana.y == t[i].y && (rana.x < t[i].x || rana.x+5 > t[i].x+10)){
                        pid_deathsound = fork() ;
                        if(pid_deathsound == 0)
                            music(death_sound) ;
                        write(p_reset[1], &reset_time, sizeof(bool)) ;
                        rana.y = maxy-4 ;
                        rana.x = maxx/2 ;
                        vite-- ;
                    }
                    //viene scritta alla rana la posizione da cambiare
                    //viene scritto al tronco che la rana è sopra (serve per non far spawnare il nemico)
                    write(p_log[1], &rana, sizeof(obj)) ;
                    write(p_onlog[i][1], &rana, sizeof(obj)) ;
                }
            }
            //viene salvata la vecchia posizione della rana
            oldfrog = rana ;
        }   
        
        //vengono salvate le vecchie posizioni dei tronchi
        for(int i = 0 ; i<4 ; i++)
            old_t[i] = t[i] ;

        //*****************************FINE RANA SUI TRONCHI************************************************************************

        //la funzione erase copia uno spazio vuoto su tutta la matrice grafica
        erase() ;
        //questa funzione stampa la mappa in background
        drawmap() ;

        //*****************************STAMPA STATISTICHE***************************************************************************

        //stampa vite, cambia colore in base a quante sono
        attron(COLOR_PAIR(1)) ;
        mvaddch(35, 1, '|') ;
        mvaddch(36, 1, '|') ;
        mvprintw(35, 2, "VITE:") ;
        attroff(COLOR_PAIR(1)) ;
        attron(A_BOLD) ;
        switch(vite){
            case 1:
                //se è una lampeggia
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
        mvprintw(36, 31, "%d", basi) ;
        attroff(A_BOLD) ;

        mvaddch(35, 50, '|') ;
        mvaddch(36, 50, '|') ;

        //stampa una barra del tempo, cambia colore in base a quanto ne rimane
        mvprintw(35, 51, "TEMPO RIMANENTE:") ;
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
        //*****************************FINE STAMPA STATISTICHE***************************************************************************

        //*****************************STAMPA VEICOLI***************************************************************************
        //i veicoli vengono stampati in modo tale che uscendo dai bordi spariscano gradualmente,
        //questo viene fatto stampando stringhe parziali dopo aver verificato quale parte del veicolo è compresa e quale no
        for(int i = 0 ; i < NUM_V ; i++){
            attron(COLOR_PAIR(21)) ;
            //MACCHINA
            if(veicolo[i].type == 'm'){
                //STA USCENDO A DESTRA
                if(veicolo[i].x+6 >= maxx){
                    for(int j = 0 ; j < 6; j++){
                        if(veicolo[i].x+j <= maxx){
                            mvaddch(veicolo[i].y-2, veicolo[i].x+j, car_sprite_h[j]) ;
                            mvaddch(veicolo[i].y-1, veicolo[i].x+j, car_sprite_m[j]) ;
                            mvaddch(veicolo[i].y, veicolo[i].x+j, car_sprite_l[j]) ;
                        }
                    }
                }
                //STA USCENDO A SINISTRA
                if(veicolo[i].x < 0){
                    for(int j = 0 ; j < 6 ; j++){
                        if(veicolo[i].x+j >= 0){
                            mvaddch(veicolo[i].y-2, veicolo[i].x+j, car_sprite_h[j]) ;
                            mvaddch(veicolo[i].y-1, veicolo[i].x+j, car_sprite_m[j]) ;
                            mvaddch(veicolo[i].y, veicolo[i].x+j, car_sprite_l[j]) ;
                        }
                    }
                }
                //E' COMPRESO
                else if(veicolo[i].x >= 0 && veicolo[i].x+6 <= maxx-1){
                    mvprintw(veicolo[i].y-2, veicolo[i].x, " ____ ") ;
                    mvprintw(veicolo[i].y-1, veicolo[i].x, "_|xx|_") ;
                    mvprintw(veicolo[i].y, veicolo[i].x, ":o--o:") ;
                }
            }
            //CAMION
            else if(veicolo[i].type == 'c'){
                //STA USCENDO A DESTRA
                if(veicolo[i].x+10 >= maxx){
                    for(int j = 0 ; j < 10; j++){
                        if(veicolo[i].x+j <= maxx){
                            mvaddch(veicolo[i].y-2, veicolo[i].x+j, cam_sprite_h[j]) ;
                            mvaddch(veicolo[i].y-1, veicolo[i].x+j, cam_sprite_m[j]) ;
                            mvaddch(veicolo[i].y, veicolo[i].x+j, cam_sprite_l[j]) ;
                        }
                    }
                }
                //STA USCENDO A SINISTRA
                if(veicolo[i].x < 0){
                    for(int j = 0 ; j < 10 ; j++){
                        if(veicolo[i].x+j >= 0){
                            mvaddch(veicolo[i].y-2, veicolo[i].x+j, cam_sprite_h[j]) ;
                            mvaddch(veicolo[i].y-1, veicolo[i].x+j, cam_sprite_m[j]) ;
                            mvaddch(veicolo[i].y, veicolo[i].x+j, cam_sprite_l[j]) ;
                        }
                    }
                }
                //E' COMPRESO
                else if(veicolo[i].x >= 0 && veicolo[i].x+10 <=maxx-1){
                    mvprintw(veicolo[i].y-2, veicolo[i].x, "__________") ;
                    mvprintw(veicolo[i].y-1, veicolo[i].x, "| x xx x |") ;
                    mvprintw(veicolo[i].y, veicolo[i].x, ":-0----0-:") ;
                }
            }
            attroff(COLOR_PAIR(21)) ;
        }
        //*****************************FINE STAMPA VEICOLI***************************************************************************

        //*****************************ALTRE STAMPE************************************************************************

        //stampa tronchi
        attron(COLOR_PAIR(21)) ;
        for(int i = 0 ; i < NUM_T ; i++){
            mvprintw(t[i].y-2, t[i].x, "|========|") ;
            mvprintw(t[i].y-1, t[i].x, "|========|") ;
            mvprintw(t[i].y, t[i].x, "|========|") ;
        }
        attroff(COLOR_PAIR(21)) ;

        //stampa nemici
        for(int i = 0 ; i < NUM_T ; i++){
            if(t[i].enemy == TRUE){
                attron(COLOR_PAIR(7)) ;
                mvprintw(t[i].y-2, t[i].x+3, "|\\/|") ;
                mvprintw(t[i].y-1, t[i].x+3, "|xx|") ;
                mvprintw(t[i].y, t[i].x+3, "(<>)") ;
                attroff(COLOR_PAIR(7)) ;
            }
        }

        //stampa proiettili nemici
        for(int i = 0 ; i < NUM_T ; i++){
            if(b_enemy[i].y >= 0 && b_enemy[i].y <= maxx-1){
                attron(COLOR_PAIR(7)) ;
                mvaddch(b_enemy[i].y, b_enemy[i].x, 'v') ;
                attroff(COLOR_PAIR(7)) ;
            }
        }

        //stampa proiettile rana
        if(bullet.y >= 0){
            attron(COLOR_PAIR(7)) ;
            mvaddch(bullet.y, bullet.x, 'o') ;
            attroff(COLOR_PAIR(7)) ;
        }

        //stampa rana
        attron(COLOR_PAIR(6)) ;
        mvprintw(rana.y-2, rana.x, "|@ @|") ;
        mvprintw(rana.y-1, rana.x, "(---)") ;
        mvprintw(rana.y, rana.x, "<(X)>") ;
        attroff(COLOR_PAIR(6)) ;
        //*****************************FINE ALTRE STAMPE************************************************************************
        
        //*****************************GESTIONE COLLISIONI************************************************************************
        
        //collisione nemico-->rana
        for(int i = 0 ; i < NUM_T ; i++){
            if(t[i].enemy && rana.y == t[i].y){
                pid_deathsound = fork() ;
                if(pid_deathsound == 0)
                    music(death_sound) ;
                collision = TRUE ;
                vite-- ;
                write(p_frog[1], &collision, sizeof(collision)) ;
                write(p_reset[1], &reset_time, sizeof(bool)) ;
            }
        }

        //collisione proiettile rana-->nemico
        for(int i = 0 ; i < NUM_T ; i++){
            if(t[i].enemy == TRUE && bullet.x >= t[i].x+3 && bullet.x <= t[i].x+6 && bullet.y <= t[i].y && bullet.y >= t[i].y-2){
                write(p_enemy[i][1], &bullet, sizeof(obj)) ;
                write(p_bulletcollision[1], &bullet, sizeof(obj)) ;
                bullet.x = -1 ;
                bullet.y = -1 ;
                score += 50 ;
            }
        }

        //collisione proiettile nemico-->rana
        for(int i = 0 ; i < NUM_T ; i++){
            if(b_enemy[i].x >= rana.x && b_enemy[i].x <= rana.x+4 && b_enemy[i].y >= rana.y-2 && b_enemy[i].y <= rana.y){
                pid_deathsound = fork() ;
                if(pid_deathsound == 0)
                    music(death_sound) ;
                write(p_ebullet[i][1], &b_enemy[i], sizeof(obj)) ;
                b_enemy[i].y = maxy ;
                collision = TRUE ;
                vite-- ;
                write(p_frog[1], &collision, sizeof(collision)) ;
                write(p_reset[1], &reset_time, sizeof(bool)) ;
            }
        }

        //collisione proiettile rana-->macchina
        for(int i = 0 ; i < NUM_V ; i++){
            if( bullet.x >= veicolo[i].x && ((veicolo[i].type == 'm' && bullet.x <= veicolo[i].x+6) || (veicolo[i].type == 'c' && bullet.x <= veicolo[i].x+9)) && bullet.y >= veicolo[i].y-2 && bullet.y <= veicolo[i].y){
                write(p_bulletcollision[1], &bullet, sizeof(obj)) ;
                bullet.x = -1 ;
                bullet.y = -1 ;
            }
        }

        //collisione proiettile nemico-->macchina
        for(int i = 0 ; i < NUM_V ; i++){
            for(int j = 0 ; j < NUM_T ; j++){
                if( b_enemy[j].x >= veicolo[i].x && ((veicolo[i].type == 'm' && b_enemy[j].x <= veicolo[i].x+6) || (veicolo[i].type == 'c' && b_enemy[j].x <= veicolo[i].x+9)) && b_enemy[j].y >= veicolo[i].y-2 && b_enemy[j].y <= veicolo[i].y){
                    write(p_ebullet[j][1], &b_enemy[j], sizeof(obj)) ;
                    b_enemy[i].y = maxy ;
                }
            }
        }

        //collisione proiettile rana<-->proiettile nemico
        for(int i = 0 ; i < NUM_T ; i++){
            if(b_enemy[i].x == bullet.x && b_enemy[i].y == bullet.y){
                write(p_ebullet[i][1], &b_enemy[i], sizeof(obj)) ;
                write(p_bulletcollision[1], &bullet, sizeof(obj)) ;
                b_enemy[i].y = maxy ;
                bullet.x = -1 ;
                bullet.y = -1 ;
            }
        }

        //collisione rana-->veicolo
        if(collision == FALSE){
            for(int i = 0 ; i < NUM_V ; i++){
                if( (veicolo[i].type == 'm' && veicolo[i].y == rana.y && ( (rana.x >= veicolo[i].x && rana.x <= veicolo[i].x+6) || (rana.x+5 >= veicolo[i].x && rana.x+5 <= veicolo[i].x+6)) || (veicolo[i].type == 'c' && veicolo[i].y == rana.y && ( (rana.x >= veicolo[i].x && rana.x <= veicolo[i].x+10) || (rana.x+5 >= veicolo[i].x && rana.x+5 <= veicolo[i].x+10)) ) )){
                    pid_deathsound = fork() ;
                    if(pid_deathsound == 0)
                        music(death_sound) ;
                    collision = TRUE ;
                    vite-- ;
                    write(p_frog[1], &collision, sizeof(collision)) ;
                    write(p_reset[1], &reset_time, sizeof(bool)) ;
                }
            }
        }
        //*****************************FINE GESTIONE COLLISIONI************************************************************************
        
        //*****************************GESTIONE BASI************************************************************************
        //il primo if controlla se la rana è all'altezza delle basei,
        //per ogni base l'if esterno controlla se la rana è compresa, l'if interno se la base è già riempita
        //e l'else interno se la base è vuota e può essere riempita,
        //l'else finale se l'altezza è quella delle basi ma la rana non è compresa totalmente/parzialmente
        //il completamento di una base riproduce un suono con un processo
        if (rana.y == 4){
            if(rana.x >= 2 && rana.x+5 <= 12){
                if(!b1_flag){
                    pid_deathsound = fork() ;
                    if(pid_deathsound == 0)
                        music(death_sound) ;
                    collision = TRUE ;
                    vite-- ;
                    write(p_frog[1], &collision, sizeof(collision)) ;
                    write(p_reset[1], &reset_time, sizeof(bool)) ;

                }
                else{
                    pid_pointsound = fork() ;
                    if(pid_pointsound == 0)
                        music(point_sound) ;
                    write(p_reset[1], &reset_time, sizeof(bool)) ;
                    b1_flag = FALSE ;
                    basi-- ;
                    score += 200 ;
                }
            }

            else if(rana.x >= 19 && rana.x+5 <= 29){
                if(!b2_flag){
                    pid_deathsound = fork() ;
                    if(pid_deathsound == 0)
                        music(death_sound) ;
                    collision = TRUE ;
                    vite-- ;
                    write(p_frog[1], &collision, sizeof(collision)) ;
                    write(p_reset[1], &reset_time, sizeof(bool)) ;
                }
                else{
                    pid_pointsound = fork() ;
                    if(pid_pointsound == 0)
                        music(point_sound) ;
                    write(p_reset[1], &reset_time, sizeof(bool)) ;
                    b2_flag = FALSE ;
                    basi-- ;
                    score += 200 ;
                }
            }

            else if(rana.x >= 36 && rana.x+5 <= 46){
                if(!b3_flag){
                    pid_deathsound = fork() ;
                    if(pid_deathsound == 0)
                        music(death_sound) ;
                    collision = TRUE ;
                    vite-- ;
                    write(p_frog[1], &collision, sizeof(collision)) ;
                    write(p_reset[1], &reset_time, sizeof(bool)) ;
                }
                else{
                    pid_pointsound = fork() ;
                    if(pid_pointsound == 0)
                        music(point_sound) ;
                    write(p_reset[1], &reset_time, sizeof(bool)) ;
                    b3_flag = FALSE ;
                    basi-- ;
                    score += 200 ;
                }
            }

            else if(rana.x >= 53 && rana.x+5 <= 63){
                if(!b4_flag){
                    pid_deathsound = fork() ;
                    if(pid_deathsound == 0)
                        music(death_sound) ;
                    collision = TRUE ;
                    vite-- ;
                    write(p_frog[1], &collision, sizeof(collision)) ;
                    write(p_reset[1], &reset_time, sizeof(bool)) ;
                }
                else{
                    pid_pointsound = fork() ;
                    if(pid_pointsound == 0)
                        music(point_sound) ;
                    write(p_reset[1], &reset_time, sizeof(bool)) ;
                    b4_flag = FALSE ;
                    basi-- ;
                    score += 200 ;
                }
            }

            else if(rana.x >= 70 && rana.x+5 <= 80){
                if(!b5_flag){
                    pid_deathsound = fork() ;
                    if(pid_deathsound == 0)
                        music(death_sound) ;
                    collision = TRUE ;
                    vite-- ;
                    write(p_frog[1], &collision, sizeof(collision)) ;
                    write(p_reset[1], &reset_time, sizeof(bool)) ;
                }
                else{
                    pid_pointsound = fork() ;
                    if(pid_pointsound == 0)
                        music(point_sound) ;
                    write(p_reset[1], &reset_time, sizeof(bool)) ;
                    b5_flag = FALSE ;
                    basi-- ;
                    score += 200 ;
                }
            }

            //se è nella stessa y ma non nella base
            else{
                collision = TRUE ;
                vite-- ;
                write(p_frog[1], &collision, sizeof(collision)) ;
                write(p_reset[1], &reset_time, sizeof(bool)) ;
            }
        }
        //*****************************FINE GESTIONE BASI************************************************************************

        //*****************************CONDIZIONI DI FINE MANCHE/GIOCO************************************************************************
        //se il tempo è uguale a 0 la rana perde una vita, viene riprodotto il suono di morte con un processo
        //e il tempo viene resettato
        if (time_left <= 0){
            pid_deathsound = fork() ;
            if(pid_deathsound == 0)
                music(death_sound) ;
            write(p_reset[1], &reset_time, sizeof(bool)) ;
            time_left = TIME_LEFT ;
            collision = TRUE;
            vite--;
            write(p_frog[1], &collision, sizeof(collision)) ;
        }

        //se le vite sono uguali a 0 esce dal ciclo di controllo con la flag di gameover
        if(vite == 0){
            exit = TRUE ;
            gameover_flag = TRUE ;
        }

        //se le basi sono uguali a 0 esce dal ciclo di controllo con la flag di win
        if(basi == 0){
            exit = TRUE ;
            win_flag = TRUE ;
        }
        //*****************************FINE CONDIZIONI DI FINE MANCHE/GIOCO************************************************************************

        box(stdscr, ACS_VLINE, ACS_HLINE) ;
        refresh() ;

        //se la flag di uscita è true, esce dal while di controllo e salva e ritorna con una struttura la flag di uscita (gameover/win) e lo score ottenuto
        if(exit){
            endval result ;
            result.score = score ;
            if(gameover_flag){
                result.result = FALSE ;
                return result ;
            }
            else if(win_flag){
                result.result = TRUE ;
                return result ;
            }
        }
    }
}