#include <iostream>
#include <vector>

using namespace std;


template <class T>
//class ArrayGen : public vector<int>{
class ArrayGen : private vector<T>{
    void troca(unsigned i,unsigned j){
        int aux;

        aux = at(i);
        vector<T>::at(i) = at(j);
        vector<T>::at(j) = aux;
    }
    //void push_back (const int &val){}
public:
    void inserirOrd(T n){
        unsigned i;

        vector<T>::push_back(n);
        for(i = size()-1;i>0;--i)
            if(vector<T>::at(i)<vector<T>::at(i-1))
                troca(i,i-1);

    }
    int size() const{
        return vector<T>::size();
    }
    /*
    int begin() const{
        return vector<T>::begin();
    }
    int end() const{
        return vector<T>::end();
    }
    */
    int at (int n){
        return vector<T>::at(n);
    }
    /*
    const int at (int n) const{
        return vector<T>::at(n);
    }
    */
    int remover(T num){
        //era pro iterador ser T
         typename std::vector<T>::iterator it = std::vector<T>::begin();
        
        while(*it != num){
            it++;
        }
        
        typename std::vector<T>::iterator ij = std::vector<T>::end();
        
        for(ij = it; ij != std::vector<T>::end(); ij++){
            *(ij) = *(ij+1);
        }
        
        //diminuir o tamanho
        return 1;
    }
    
    T& operator[](unsigned n){
        return vector<T>::at(n);
    }
    
    void inserirMeio(T num){
        
        //TODO aumentar o tamanho do array
        int meio = size() /2;
        
        for(int i = size()-1; i > meio; i--){
            troca(i,i-1);
        }
        
        vector<T>::at(meio) = num;
        
    }
};
int main()
{
    ArrayGen<int> V;
    int i;

    V.inserirOrd(50);
    V.inserirOrd(10);
    V.inserirOrd(30);
    V.inserirOrd(40);
    V.inserirOrd(60);
    V.inserirOrd(100);

    //V.push_back(0);
    for(i=0;i<V.size();++i)
        cout<<V.at(i)<<' ';
    cout<<endl;
    
    V.inserirMeio(90);
    
    V[0] = 1;
    
    
    for(i=0;i<V.size();++i)
        cout<<V.at(i)<<' ';
    cout<<endl;
    
    V.remover(40);
    
    
    for(i=0;i<V.size();++i)
        cout<<V.at(i)<<' ';
    cout<<endl;

    cout << "Hello world!" << endl;
    return 0;
}











