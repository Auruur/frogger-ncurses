#include "all.h"
#include "global.h"

bool menu(){
    clear() ;
    int option = 1 ;
    diff = 1 ;
    int s, c ;
    wbkgd(stdscr, COLOR_PAIR(23)) ;
    box(stdscr, ACS_VLINE, ACS_HLINE) ;
    title_maker(option) ;
    graphic_frog() ;
    refresh() ;
    
    //Nel while ci sono una serie di switch e if annidati che permettono di navigare nelle opzioni del menu (play/exit e difficoltà)
    while(true){
        attron(COLOR_PAIR(23)) ;
        mvprintw(21, 24, "PRESS SPACEBAR TO START PLAYING!") ;
        attroff(COLOR_PAIR(23)) ;
        s =  getch() ;
        switch(s){
            case KEY_LEFT:
                option = 1 ;
                title_maker(option) ;
                break ;          

            case KEY_RIGHT:
                option = 2 ;
                title_maker(option) ;
                break ;
            
            case SPACE_BAR:
                if(option == 1){
                    difficulty_maker(diff) ;
                    while(true){
                        switch(diff){
                            case 1:
                                c = getch() ;
                                switch(c){
                                    case KEY_UP:
                                        diff = 1 ;
                                        difficulty_maker(diff) ;
                                        break ;
                                    case KEY_DOWN:
                                        diff = 2 ;
                                        difficulty_maker(diff) ;
                                        break ;
                                    case SPACE_BAR:
                                            return TRUE ;
                                }
                                break ;
                            case 2:
                                c = getch() ;
                                switch(c){
                                    case KEY_UP:
                                        diff = 1 ;
                                        difficulty_maker(diff) ;
                                        break ;
                                    case KEY_DOWN:
                                        diff = 3 ;
                                        difficulty_maker(diff) ;
                                        break ;
                                    case SPACE_BAR:
                                            return TRUE ;
                                }
                                break ;
                            case 3:
                                c = getch() ;
                                switch(c){
                                    case KEY_UP:
                                        diff = 2 ;
                                        difficulty_maker(diff) ;
                                        break ;
                                    case KEY_DOWN:
                                        diff = 3 ;
                                        difficulty_maker(diff) ;
                                        break ;
                                    case SPACE_BAR:
                                            return TRUE ;
                                }
                                break ;

                        }
                    }
                }
                else
                    return FALSE ;
                break ;
        }
    }
}

void title_maker(int option){
    if(option == 1)
        attron(COLOR_PAIR(11)) ;
    else
        attron(COLOR_PAIR(10)) ;
    
    //LETTERA P
    mvprintw(3, 13, "      ") ;
    mvprintw(4,13, "  ") ;
    mvprintw(4, 17, "  ") ;
    mvprintw(5, 13, "      ") ;
    mvprintw(6, 13, "  ") ;
    mvprintw(7, 13, "  ") ;
    //LETTERA L
    mvprintw(9, 13, "  ") ;
    mvprintw(10, 13, "  ") ;
    mvprintw(11, 13, "  ") ;
    mvprintw(12, 13, "  ") ;
    mvprintw(13, 13, "      ") ;
    //LETTERA A
    mvprintw(15, 13, "      ") ;
    mvprintw(16, 13, "  ") ;
    mvprintw(16, 17, "  ") ;
    mvprintw(17, 13, "      ") ;
    mvprintw(18, 13, "  ") ;
    mvprintw(18, 17, "  ") ;
    mvprintw(19, 13, "  ") ;
    mvprintw(19, 17, "  ") ;
    //LETTERA Y
    mvprintw(21, 13, "  ") ;
    mvprintw(21, 17, "  ") ;    
    mvprintw(22, 13, "      ") ;
    mvprintw(23, 15, "  ") ;
    mvprintw(24, 15, "  ") ;
    mvprintw(25, 15, "  ") ;

    if(option == 1)
        attron(COLOR_PAIR(10)) ;
    else
        attron(COLOR_PAIR(11)) ;
    
    //LETTERA E
    mvprintw(3, 61, "      ") ;
    mvprintw(4, 61, "  ") ;
    mvprintw(5, 61, "      ") ;
    mvprintw(6, 61, "  ") ;
    mvprintw(7, 61, "      ") ;
    //LETTERA X
    mvprintw(9, 61, "  ") ;
    mvprintw(9, 65, "  ") ;
    mvprintw(10, 62, "    ") ;
    mvprintw(11, 63, "  ") ;
    mvprintw(12, 62, "    ") ;
    mvprintw(13, 61, "  ") ;
    mvprintw(13, 65, "  ") ;
    //LETTERA I
    for(int i = 15; i < 20 ; i++)
        mvprintw(i, 63, "  ") ;
    //LETTERA T
    mvprintw(21, 61, "      ") ;
    for(int i = 22; i < 26; i++)
        mvprintw(i, 63, "  ") ;

}


void graphic_frog(){
    //stampa del disegno della rana
    attron(COLOR_PAIR(21)) ;
    mvprintw(4, 28, "      ") ;
    mvprintw(4, 46, "      ") ;
    mvprintw(5, 26, "  ") ;
    mvprintw(5, 34, "  ") ;
    mvprintw(5, 44, "  ") ;
    mvprintw(5, 52, "  ") ;
    mvaddch(6, 25, ' ') ;
    mvprintw(6, 28, "    ") ;
    mvprintw(6, 36, "        ") ;
    mvprintw(6, 48, "    ") ;
    mvaddch(6, 54, ' ') ;
    mvaddch(7, 25, ' ') ;
    mvprintw(7, 28, "    ") ;
    mvprintw(7, 48, "    ") ;
    mvaddch(7, 54, ' ') ;   
    mvaddch(8, 25, ' ') ;
    mvaddch(8, 54, ' ') ;   
    mvprintw(9, 26, "  ") ;
    mvprintw(9, 52, "  ") ;   
    mvprintw(10, 24, "  ") ;
    mvprintw(10, 54, "  ") ;
    mvaddch(11, 23, ' ') ;
    mvaddch(11, 29, ' ') ;
    mvaddch(11, 50, ' ') ;
    mvaddch(11, 56, ' ') ;
    mvaddch(12, 23, ' ') ;
    mvprintw(12, 30, "                    ") ;
    mvaddch(12, 56, ' ') ;
    mvprintw(13, 24, "  ") ;
    mvprintw(13, 54, "  ") ;
    mvprintw(14, 26, "      ") ;
    mvprintw(14, 48, "      ") ;
    mvaddch(15, 28, ' ') ;
    mvaddch(15, 51, ' ') ;
    mvprintw(16, 24, "    ") ;
    mvprintw(16, 52, "    ") ;
    mvaddch(17, 23, ' ') ;
    mvaddch(17, 27, ' ') ;
    mvaddch(17, 52, ' ') ;
    mvaddch(17, 56, ' ') ;
    mvaddch(18, 23, ' ') ;
    mvprintw(18, 30, "  ") ;
    mvprintw(18, 34, "  ") ;
    mvprintw(18, 44, "  ") ;
    mvprintw(18, 48, "  ") ;
    mvaddch(18, 56, ' ') ;
    mvprintw(19, 24, "      ") ;
    mvprintw(19, 32, "  ") ;
    mvprintw(19, 36, "        ") ;
    mvprintw(19, 46, "  ") ;
    mvprintw(19, 50, "      ") ;
    attroff(COLOR_PAIR(21)) ;

    attron(COLOR_PAIR(20)) ;
    mvprintw(5, 28, "      ") ;
    mvprintw(5, 46, "      ") ;
    mvprintw(6, 26, "  ") ;
    mvprintw(6, 32, "    ") ;
    mvprintw(6, 44, "    ") ;
    mvprintw(6, 52, "  ") ;
    mvprintw(7, 26, "  ") ;
    mvprintw(7, 32, "    ") ;
    mvprintw(7, 44, "    ") ;
    mvprintw(7, 52, "  ") ;
    mvprintw(8, 26, "          ") ;
    mvprintw(8, 44, "          ") ;
    mvprintw(9, 28, "      ") ;
    mvprintw(9, 46, "      ") ;
    mvprintw(16, 38, "    ") ;
    mvprintw(17, 37, "      ") ;
    mvprintw(18, 36, "        ") ;
    attroff(COLOR_PAIR(20)) ;

    attron(COLOR_PAIR(22)) ;
    mvprintw(7, 36, "        ") ;
    mvprintw(8, 36, "        ") ;
    mvprintw(9, 34, "            ") ;
    mvprintw(10, 26, "                            ") ;
    mvprintw(11, 24, "     ") ;
    mvprintw(11, 30, "                    ") ;
    mvprintw(11, 51, "     ") ;
    mvprintw(12, 24, "      ") ;
    mvprintw(12, 50, "      ") ;
    mvprintw(13, 26, "                            ") ;
    mvprintw(14, 32, "                ") ;
    mvprintw(15, 29, "                      ") ;
    mvprintw(16, 28, "          ") ;
    mvprintw(16, 42, "          ") ;
    mvprintw(17, 24, "   ") ;
    mvprintw(17, 28, "         ") ;
    mvprintw(17, 43, "         ") ;
    mvprintw(17, 53, "   ") ;
    mvprintw(18, 24, "      ") ;
    mvprintw(18, 32, "  ") ;
    mvprintw(18, 46, "  ") ;
    mvprintw(18, 50, "      ") ;
    attroff(COLOR_PAIR(22)) ;
}

void difficulty_maker(int diff_param){
    erase() ;

    if(diff_param == 1)
        attron(COLOR_PAIR(11)) ;
    else
        attron(COLOR_PAIR(10)) ;

    //LETTERA E
    mvprintw(10, 26, "      ") ;
    mvprintw(11, 26, "  ") ;
    mvprintw(12, 26, "      ") ;
    mvprintw(13, 26, "  ") ;
    mvprintw(14, 26, "      ") ;
    //LETTERA A
    mvprintw(10, 34, "      ") ;
    mvprintw(11, 34, "  ") ;
    mvprintw(11, 38, "  ") ;
    mvprintw(12, 34, "      ") ;
    mvprintw(13, 34, "  ") ;
    mvprintw(13, 38, "  ") ;
    mvprintw(14, 34, "  ") ;
    mvprintw(14, 38, "  ") ;
    //LETTERA S
    mvprintw(10, 42, "      ") ;
    mvprintw(11, 42, "    ") ;
    mvprintw(12, 42, "      ") ;
    mvprintw(13, 44, "    ") ;
    mvprintw(14, 42, "      ") ;
    //LETTERA Y
    mvprintw(10, 50, "  ") ;
    mvprintw(10, 54, "  ") ;    
    mvprintw(11, 50, "      ") ;
    mvprintw(12, 52, "  ") ;
    mvprintw(13, 52, "  ") ;
    mvprintw(14, 52, "  ") ;

    if(diff_param == 2)
        attron(COLOR_PAIR(27)) ;
    else
        attron(COLOR_PAIR(10)) ;

    //LETTERA M
    mvaddch(17, 18, ' ') ;
    mvaddch(17, 23, ' ') ;
    mvprintw(18, 18, "  ") ;
    mvprintw(18, 22, "  ") ;
    mvprintw(19, 18, "      ") ;
    mvprintw(20, 18, "  ") ;
    mvprintw(20, 22, "  ") ;
    mvprintw(21, 18, "  ") ;
    mvprintw(21, 22, "  ") ;
    //LETTERA E
    mvprintw(17, 26, "      ") ;
    mvprintw(18, 26, "  ") ;
    mvprintw(19, 26, "      ") ;
    mvprintw(20, 26, "  ") ;
    mvprintw(21, 26, "      ") ;
    //LETTERA D
    mvprintw(17, 34, "      ") ;
    mvprintw(18, 34, "   ") ;
    mvprintw(18, 38, "  ") ;
    mvprintw(19, 34, "   ") ;
    mvprintw(19, 38, "  ") ;
    mvprintw(20, 34, "   ") ;
    mvprintw(20, 38, "  ") ;
    mvprintw(21, 34, "      ") ;
    //LETTERA I
    for(int i = 17; i < 22 ; i++)
        mvprintw(i, 44, "  ") ;
    //LETTERA U
    mvprintw(17, 50, "  ") ;
    mvprintw(17, 54, "  ") ;
    mvprintw(18, 50, "  ") ;
    mvprintw(18, 54, "  ") ;
    mvprintw(19, 50, "  ") ;
    mvprintw(19, 54, "  ") ;
    mvprintw(20, 50, "  ") ;
    mvprintw(20, 54, "  ") ;
    mvprintw(21, 50, "      ") ;
    //LETTERA M
    mvaddch(17, 58, ' ') ;
    mvaddch(17, 63, ' ') ;
    mvprintw(18, 58, "  ") ;
    mvprintw(18, 62, "  ") ;
    mvprintw(19, 58, "      ") ;
    mvprintw(20, 58, "  ") ;
    mvprintw(20, 62, "  ") ;
    mvprintw(21, 58, "  ") ;
    mvprintw(21, 62, "  ") ;

    if(diff_param == 3)
        attron(COLOR_PAIR(28)) ;
    else
        attron(COLOR_PAIR(10)) ;

    //LETTERA H
    mvprintw(24, 26, "  ") ;
    mvprintw(24, 30, "  ") ;
    mvprintw(25, 26, "  ") ;
    mvprintw(25, 30, "  ") ;
    mvprintw(26, 26, "      ") ;
    mvprintw(27, 26, "  ") ;
    mvprintw(27, 30, "  ") ;
    mvprintw(28, 26, "  ") ;
    mvprintw(28, 30, "  ") ;
    //LETTERA A
    mvprintw(24, 34, "      ") ;
    mvprintw(25, 34, "  ") ;
    mvprintw(25, 38, "  ") ;
    mvprintw(26, 34, "      ") ;
    mvprintw(27, 34, "  ") ;
    mvprintw(27, 38, "  ") ;
    mvprintw(28, 34, "  ") ;
    mvprintw(28, 38, "  ") ;
    //LETTERA R
    mvprintw(24, 42, "      ") ;
    mvprintw(25, 42, "   ") ;
    mvprintw(25, 46, "  ") ;
    mvprintw(26, 42, "      ") ;
    mvprintw(27, 42, "   ") ;
    mvprintw(27, 46, "  ") ;
    mvprintw(28, 42, "   ") ;
    mvprintw(28, 46, "  ") ;
    //LETTERA D
    mvprintw(24, 50, "      ") ;
    mvprintw(25, 50, "   ") ;
    mvprintw(25, 54, "  ") ;
    mvprintw(26, 50, "   ") ;
    mvprintw(26, 54, "  ") ;
    mvprintw(27, 50, "   ") ;
    mvprintw(27, 54, "  ") ;
    mvprintw(28, 50, "      ") ;

    if(diff == 1)
        attron(COLOR_PAIR(11)) ;
    else if(diff == 2)
        attron(COLOR_PAIR(27)) ;
    else if(diff == 3)
        attron(COLOR_PAIR(28)) ;

    box(stdscr, ACS_VLINE, ACS_HLINE) ;

    refresh() ;
}