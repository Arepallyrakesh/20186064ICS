#include "csapp.h"
// #include<stdlib.h>

int main()
{
    pid_t pid; /* the pid is to get the process id of the process */


    /* child will be in a sleep state untill the signal is received and then it will die*/
     if ((pid = Fork()) == 0) {
        Pause(); /* Wait untill the signal arrives*/
        printf("control should never reach here!\n");
        exit(0);
    }

    /*Parent process sends a SIGKILL signal to the child*/
    Kill(pid, SIGKILL);
    exit(0);
}
