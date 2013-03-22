#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <string>
#include <iostream>
#include "Draw.h"
using namespace std;
int main(){
	Draw *d = new Draw;
	string input;
	do {
		getline(cin,input);
		if (!input.compare("line"))
			d->draw(Draw::line);
		else if (!input.compare("triangle"))
			d->draw(Draw::triangle);
		else if (!input.compare("square"))
			d->draw(Draw::square);
		else if (!input.compare("better square"))
			d->draw(Draw::better);
		else if (!input.compare("sky"))
			d->draw(Draw::sky);
	}while(input.compare("quit") != 0);

	delete d;
	return 0;
}
