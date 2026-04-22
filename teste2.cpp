#include <iostream>
#include <vector>

using namespace std;

//template <class T>
class Objeto {


public:
	virtual float volume() = 0;

};

class Triangulo : public Objeto {
	float h,b;





public:



	Triangulo() {
	    b = 1;
	    h =1;
	}


	virtual float volume()  {
		return (b*h)/3;
	}

};

class Cubo : public Objeto {
	float l;

public:
Cubo(){
	l=1;
}
	float volume() {
		return l*l*l;

	}
};

class ArrayDeFigs : public vector<Objeto*> {

public:


	float volumeTotal() {

		vector<Objeto*>::iterator it;
		float vt = 0;


		for (it= begin(); it < end(); it++)
		{
			vt += (*it)->volume();
		}

		return vt;


	}
};
int main() {

	ArrayDeFigs arara;


	arara.push_back(new Triangulo);
	arara.push_back(new Cubo);
	cout << (arara.at(0))->volume();
	return 0;
}