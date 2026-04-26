#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// #define frame 0.25
void esperar(float tempo)
{

    time_t current_time = 0; // A variable to save the time

    current_time = clock();
    // wating 2 seconds
    for (; (clock() - current_time) < (tempo * CLOCKS_PER_SEC);)
        ;
}
int main(int argc, char *argv[])
{
    float frame = *argv[1];
    for (int i = 0; i < *argv[2]; i++)

    {

        printf("\n  ٩(◕‿◕｡)۶  ");
        esperar(frame);
        system("clear");
        printf("\n  d(◕‿◕｡)d  ");
        esperar(frame);
        system("clear");
        printf("\n  ٩(｡◕‿◕)۶  ");
        esperar(frame);
        system("clear");
        printf("\n  b(｡◕‿◕)b  ");
        esperar(frame);
        system("clear");
    }
    return 0;
}