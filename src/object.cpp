#include <string>

using namespace std;

class Object{
	private:
		string tagName;
		int posX;
		int posY;
	public:
		Object(){}

		Object(string tagName_){
			tagName = tagName_;
		}

		Object(string tagName_, int posX_, int posY_){
			tagName = tagName_;
			posX = posX_;
			posY = posY_;
		}

		string getTagName(){
			return tagName;
		}
};