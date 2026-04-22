#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int n, count = 0, dif, difcount = 0;

    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {

        int pid;
        pid = fork();
        count++;

        if(dif != getpid()){
            difcount++;
        }
        dif = getpid();

        printf("\n%d", getpid());
    }
    printf("\n contagem %d\n", count);
    printf("\n contagem de diferentes %d\n", difcount);
    return 0;
}