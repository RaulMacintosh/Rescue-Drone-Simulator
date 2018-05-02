#include <iostream>
#include <locale>

class Object{
	private:
		wchar_t tagName;
		int posX;
		int posY;
	public:
		Object(){}

		Object(wchar_t tagName_){
			tagName = tagName_;
		}

		Object(wchar_t tagName_, int posX_, int posY_){
			tagName = tagName_;
			posX = posX_;
			posY = posY_;
		}

		wchar_t getTagName(){
			return tagName;
		}

		int getX(){
			return posX;
		}

		int getY(){
			return posY;
		}

		void moveLeft(){
			posY -= 1;
		}

		void moveRight(){
			posY += 1;
		}

		void moveDown(){
			posX -= 1;
		}

		void moveUp(){
			posX += 1;
		}
};