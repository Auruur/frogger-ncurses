#include "all.h"
#include "global.h"

//questa funzione utilizza un thread che effettua
//la chiamata di sistema aplay per riprodurre un suono,
//specificato dal path passato come parametro
void* music_routine(char* sound){
    char command[256] ;
    sprintf(command, "aplay %s ", sound) ;
    system(strcat(command,"1>/dev/null 2>/dev/null"));
    pthread_exit(0) ;
}