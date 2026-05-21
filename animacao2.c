#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define frame 0.25
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

        printf("\n           (|__/)             ");
        printf("\n3O____     (•ㅅ•)             ");
        printf("\n      |____ _||__ ____         ");
        printf("\n           |     |    |____      ");
        printf("\n           |     |         O3   ");
        printf("\n           |     /             ");
        printf("\n            |   /             ");
        printf("\n             |_/             ");
        printf("\n            /   |           ");
        printf("\n           /     |           ");
        printf("\n           |      |         ");
        printf("\n           |     |          ");
        printf("\n         mM       Mm       ");
        esperar(frame);
        system("clear");


        printf("\n           (|__/)             ");
        printf("\n           (•ㅅ•)             ");
        printf("\n3O________ __||__ _________O3   ");
        printf("\n           |     |            ");
        printf("\n           |     |            ");
        printf("\n           |     /             ");
        printf("\n            |   /             ");
        printf("\n             |_/             ");
        printf("\n            /   |           ");
        printf("\n           /     |           ");
        printf("\n           |      |         ");
        printf("\n           |     |          ");
        printf("\n         mM       Mm       ");
        esperar(frame);
        system("clear");


        printf("\n           (|__/)             ");
        printf("\n           (•ㅅ•)      ____O3  ");
        printf("\n       ____ _||__ ____|           ");
        printf("\n3O____|    |     |            ");
        printf("\n           |     |            ");
        printf("\n           |     /             ");
        printf("\n            |   /             ");
        printf("\n             |_/             ");
        printf("\n            /   |           ");
        printf("\n           /     |           ");
        printf("\n           |      |         ");
        printf("\n           |     |          ");
        printf("\n         mM       Mm       ");
        esperar(frame);
        system("clear");


        printf("\n           (|__/)             ");
        printf("\n           (•ㅅ•)             ");
        printf("\n3O________ _||__ _________O3   ");
        printf("\n           |     |            ");
        printf("\n           |     |            ");
        printf("\n           |     /             ");
        printf("\n            |   /             ");
        printf("\n             |_/             ");
        printf("\n            /   |           ");
        printf("\n           /     |           ");
        printf("\n           |      |         ");
        printf("\n           |     |          ");
        printf("\n         mM       Mm       ");
        esperar(frame);
        system("clear");


    }
    return 0;
}