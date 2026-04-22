
#include <stdio.h>

int main(){

    for (int  i = 0; i < 40; i++)
    {
      printf("\x1b[%dmTexto %d\n",i,i);
    }
    


    printf("\x1b[0mTexto branco\n");
    printf("\x1b[5mTexto \x1b[0m\n");
    printf("\x1b[27mTexto \x1b[0m\n");
    printf("\x1b[28mTexto \x1b[0m\n");
    printf("\x1b[30mTexto \x1b[0m\n");
    printf("\x1b[29mTexto \x1b[0m\n");
    printf("\x1b[30mTexto \x1b[0m\n");
    printf("\x1b[31mTexto Vermelho\x1b[0m\n");
    printf("\x1b[32mTexto Verde\x1b[0m\n");
    printf("\x1b[33mTexto Amarelo\x1b[0m\n");
    printf("\x1b[34mTexto Azul\x1b[0m\n");
    printf("\x1b[35mTexto roxo\x1b[0m\n");

    return 0;
}