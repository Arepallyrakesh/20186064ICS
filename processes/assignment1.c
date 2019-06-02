#include <stdio.h>
unsigned int sleep(unsigned int secs);
unsigned int snooze(unsigned int secs)
{
    unsigned sleep_secs;
    sleep_secs = sleep(secs);
    printf("Slept for %u of %u secs\n", secs - sleep_secs, secs);
    return sleep_secs;
}

int main(void)
{
    snooze(3);
    return 0;
}