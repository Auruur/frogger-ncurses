//inclusione dei file che contengono le librerie, le variabili globali e i prototipi di funzione 
#include "all.h"
#include "global.h"

void timer(int p_time[2], int p_reset[2]){
    int time_left = TIME_LEFT ;
    bool reset_time = FALSE ;
    while(true){
        //lettura della pipe 
        read(p_reset[0], &reset_time, sizeof(bool)) ;
        //la pipe manda un segnale quando il tempo e' da resettare
        //il timer viene resettato al valore di partenza e il segnale messo a false
        if(reset_time == TRUE){
            reset_time = FALSE ;
            time_left = TIME_LEFT ;
        }
        write(p_time[1], &time_left, sizeof(int)) ;
        time_left-- ; //scorrimento del tempo del timer 
        sleep(1) ;
    }
}
