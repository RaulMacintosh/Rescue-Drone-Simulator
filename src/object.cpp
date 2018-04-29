class Object{
	private:
		char tagName;
		int posX;
		int posY;
	public:
		Object(){}

		Object(char tagName_){
			tagName = tagName_;
		}

		Object(char tagName_, int posX_, int posY_){
			tagName = tagName_;
			posX = posX_;
			posY = posY_;
		}

		char getTagName(){
			return tagName;
		}

		int getX(){
			return posX;
		}

		int getY(){
			return posY;
		}
};