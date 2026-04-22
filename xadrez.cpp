#include <iostream>
using namespace std;

class Tabuleiro {
    int x, y;
    int coord[x][y];
    
    Tabuleiro(){}
    
    Tabuleiro(int x, int y){
        //sla
        coord[x][y];
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                coord[i][j] = 0;
            }
        }
    }
};

class Peca{
    //classe abstrata

    public:
    virtual int cor;
    virtual int coord[2];


};

int main(){

    return 0;
}
