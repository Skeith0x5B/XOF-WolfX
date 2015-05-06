#include <iostream>
using namespace std;


#include "../Rendering/XOF_MaterialManager.hpp"


int main() {
	cout << "TESTING" << endl;

	MaterialManager *manager = &MaterialManager::Get();
	manager->StartUp();

	MaterialDesc mat;
	mat.textures[0] = "Texture name";
	mat.shaders[0] = "Shader name";

	manager->AddMaterial( "myMaterial", mat );
	SharedMaterialPtr myMaterial = manager->GetMaterial( "myMaterial" );

	int stop = 1;

	system("PAUSE");
	return 0;
}