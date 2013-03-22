/*
 * Draw.h
 *
 *  Created on: Dec 9, 2012
 *      Author: alx
 */

#ifndef DRAW_H_
#define DRAW_H_

class Draw {
public:
	enum Type{
		line,
		triangle,
		square,
		better,
		sky
	};
	Draw();
	virtual ~Draw();

	void draw(Type type);

private:
	void setupWindow();
	void setupGL();
	void drawLine();
	void drawTriangle();
	void drawSquare();
	void drawBetterSquare();
	void drawSky();

private:
	SDL_Surface *window;
	int width;
	int height;
};

#endif /* DRAW_H_ */
