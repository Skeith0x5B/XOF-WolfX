#include <iostream>
using namespace std;


#include "../Rendering/XOF_SpriteManager.hpp"


int main() {
	cout << "TESTING" << endl;

	SpriteManager *manager = SpriteManager::Get();
	manager->StartUp();

	SpriteAnimationParams params;
	params.frameCount = 13;
	params.startingFrame = 6;
	params.subAnimationFrameCount = 3;

	manager->AddSprite( "mySprite", &params );
	Sprite *sprite = manager->GetSprite( "mySprite" );

	int stop = 1;

	system("PAUSE");
	return 0;
}