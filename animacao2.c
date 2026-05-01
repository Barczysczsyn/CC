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

        printf("\n           (\__/)             ");
        printf("\n           (•ㅅ•)             ");
        printf("\n           __||__             ");
        printf("\n           |     |             ");
        printf("\n           |     |             ");
        printf("\n           \     /             ");
        printf("\n            \   /             ");
        printf("\n             \_/             ");
        printf("\n            /   \           ");
        printf("\n           /     \           ");
        printf("\n           |      |         ");
        printf("\n           |     |          ");
        printf("\n         mM       Mm       ");
        printf("\n                            ");
        printf("\n                            ");
        esperar(frame);
        system("clear");
    }
    return 0;
}