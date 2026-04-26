#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void esperar(float tempo)
{

    time_t current_time = 0; // A variable to save the time

    current_time = clock();
    // wating 2 seconds
    for (; (clock() - current_time) < (tempo * CLOCKS_PER_SEC);)
        ;
}
int main()
{
    while (1)
    {

        printf("\n  ٩(◕‿◕｡)۶  ");
        esperar(0.25);
        system("clear");
        printf("\n  d(◕‿◕｡)d  ");
        esperar(0.25);
        system("clear");
        printf("\n  ٩(｡◕‿◕)۶  ");
        esperar(0.25);
        system("clear");
        printf("\n  b(｡◕‿◕)b  ");
        esperar(0.25);
        system("clear");
    }
    return 0;
}