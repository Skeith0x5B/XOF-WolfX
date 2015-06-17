#include "WolfX.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

// RENDERING
#include "../Platform/XOF_Platform.hpp"
#include "../Platform/XOF_PlatformInfo.hpp"
#include "../Platform/XOF_Timer.hpp"
#include "../Rendering/XOF_Renderer_OpenGL.hpp"
#include "../Rendering/XOF_FirstPersonCamera.hpp"
#include "../Rendering/XOF_GeoPrimitiveGenerator.hpp"
#include "../Rendering/XOF_Transform.hpp"
#include "../Rendering/XOF_Shader.hpp"

#include "../Rendering/XOF_MaterialManager.hpp"
MaterialManager *materialManager;
#include "../Rendering/XOF_SpriteManager.hpp"
SpriteManager *spriteManager;
#include "../Rendering/XOF_MeshManager.hpp"
MeshManager *meshManager;

// AUDIO
#include "../Audio/XOF_Music.hpp"
Music gTempBgm;
#include "../Audio/XOF_SoundEffect.hpp"
SoundEffect gTempSoundFX;


// GAME RESOURCE PATHS
#include "WolfX_ResourcePaths.hpp"


// TEMP
//#include "../GameplayFoundations/XOF_EngineSubSystemFrontEnd.hpp"
EngineSubSystems *engineSystems;


struct TempLevel {
	Texture	levelMap;
	Mesh levelGeom;
	UniqueMeshPtr levelPtr;
	std::vector<Transform> tileTransforms;
	Material *tileMaterial;
	FirstPersonCamera *playerView;
};


enum WOLFX_LEVEL_OBJECTS {
	BONES = 0,
	LIGHT,
	TURKEY,
	AMMO,
	SPRITE_COUNT = 5,
	DOOR,
	ENEMY,
};


// Game objects
#include "../GameplayFoundations/XOF_GameObject.hpp"
#include "WolfX_GameObjects.hpp"
#include <list>
std::list<UniqueGameObjectPtr> gGameObjects;
std::unique_ptr<Player> gTempPlayer = nullptr;

// Game events
#include "WolfX_Events.hpp"
#include <queue>
std::queue<Event> gGameEvents;


FirstPersonCamera gCamera;


// Temporary WolfX level handling
const static U32 CELL_WIDTH = 1, CELL_HEIGHT = 1, CELL_LENGTH = 1;
static const U32 ATLAS_VERT_ELE_COUNT = 2;
static const U32 ATLAS_HORIZ_ELE_COUNT = 2;

static void PlaceGuard( U32 xCoord, U32 yCoord );
// Door code specifies the positioning of the door, ADD VECTOR FOR MOVER TO MOVE ALONG
static void PlaceDoor( U32 xCoord, U32 yCoord, U32 doorCode );
static void PlaceLight( U32 xCoord, U32 yCoord );

TempLevel gTempLevel;
void LoadTempLevel( const std::string &fileName );


WolfX::WolfX() {
}

WolfX::~WolfX() {
	materialManager->ShutDown();
	mRenderer->ShutDown();
}

bool WolfX::Initialise( U16 width, U16 height, CHAR* caption ) {
	if( !Game::Initialise( width, height, caption ) ) {
		return false;
	}

	mMouse.SetWindow( mWindow.get() );

	mEngine.renderer.reset( new Renderer() );
	mEngine.renderer->StartUp();
	mEngine.audio.reset( new AudioEngine() );
	mEngine.audio->Startup();
	engineSystems = &mEngine;

	mEngine.audio->SubmitAudioRequest( AudioRequest(
		XOF_AUDIO_REQUEST_TYPE::PLAY_MUSIC, nullptr, nullptr,
		50.f, WOLFX_RESOURCE::AUDIO + "BGM.ogg" ) );

	SetupScene();

	mIsInitialised = true;
	return mIsInitialised;
}

// frame-limit
static double FRAME_TIME = 1.0 / 120;

void WolfX::Run() {
	if( mIsInitialised ) {
		Timer timer;
		timer.Reset();
		
		double lastTime = timer.GetMilliseconds();
		double unprocessedTime = 0.0;
		double elapsedTime = 0.0;
		double timeDelta = 0.0;
		U32 frameCount = 0;

		gGameEvents.push( PlayerSpawnedEvent() );

		while( mWindow->isOpen() ) {
			double currentTime = timer.GetMilliseconds();
			timeDelta = currentTime - lastTime;
			lastTime = currentTime;

			unprocessedTime += timeDelta;

			HandleEvents(); // System events (close, resize etc...)
			HandleInput();

			// TODO: FIX LOOP - MAY INVOLVE MODIFYING TIMER CLASS (SEE OGRE IMPLEMENTATION)
			//while( unprocessedTime >= FRAME_TIME ) {
				// GAME LOGIC - REACT TO EVENTS
				while( !gGameEvents.empty() ) {
					Event &e = gGameEvents.front();
					for( auto &object : gGameObjects ) {
						object->OnEvent( &e );
					}
					// TEMP - REMOVE
					gTempPlayer->OnEvent( &e );
					gGameEvents.pop();
				}

				// GAME LOGIC - UPDATE/STEP GAME OBJECTS
				for( auto &object : gGameObjects ) {
					object->Update( static_cast<float>( timeDelta ) );
				}
				gTempPlayer->Update( static_cast<float>( timeDelta ) );

				// ENGINE LOGIC - Process subsystem requests
				mEngine.renderer->RenderFrame();
				mEngine.audio->Update();
				//mAudio.Update( dt ) etc...

				++frameCount;
				elapsedTime += timeDelta;
				if( elapsedTime >= 1000 ) {
					std::cout << "FPS: " << frameCount << std::endl;
					frameCount = 0;
					elapsedTime = 0.0;
				}
				unprocessedTime -= FRAME_TIME;
			//}
			Render();
		} // while( mWindow->isOpen() ) 
	} // if( mIsInitialised )
}


#include "../Audio/XOF_AudioManager.hpp"
std::unique_ptr<Mesh> booker;
void WolfX::SetupScene() {
	materialManager = MaterialManager::Get();
	materialManager->StartUp();

	spriteManager = SpriteManager::Get();
	spriteManager->StartUp();

	materialManager->GetMaterial( "DEFAULT_MATERIAL" )->GetShader()->AddUniform( "transform" );
	materialManager->GetMaterial( "DEFAULT_MATERIAL" )->GetShader()->Bind();

	// Add Wolfenstein sprite-sheets
	MaterialDesc sheet;
	sheet.diffuseMaps[0] = WOLFX_RESOURCE::TEXTURE + "ObjectSheet_2.png";
	sheet.shader = XOF_RESOURCE::SHADER + "DiffuseTexture";

	sheet.gpuStateName = "lightGPUState";
	sheet.gpuState.AddToDisabledFlags( XOF_GPU_STATE_FLAGS::CULLING );
	sheet.gpuState.AddToDisabledFlags( XOF_GPU_STATE_FLAGS::DEPTH_TEST );
	sheet.gpuState.AddToDisabledFlags( XOF_GPU_STATE_FLAGS::DEPTH_CLAMP );

	Material *sprsht = materialManager->AddMaterial( "objectSheet", sheet );

	// TODO: Add player weapon...
	MaterialDesc playerWeaponMatDesc;
	playerWeaponMatDesc.diffuseMaps[0] = WOLFX_RESOURCE::TEXTURE + "PistolSpriteSheet.png";
	playerWeaponMatDesc.shader = XOF_RESOURCE::SHADER + "DiffuseTexture";
	playerWeaponMatDesc.gpuStateName = "lightGPUState";
	Material *tempTest = materialManager->AddMaterial( "playerWeaponMaterial", playerWeaponMatDesc );
	
	SpriteAnimationParams weaponParams;
	weaponParams.startingFrame = 0;
	weaponParams.frameCount = 5;
	weaponParams.subAnimationFrameCount = 5;
	Sprite *weapon = spriteManager->AddSprite( "weapon", &weaponParams );

	// Lights
	SpriteAnimationParams params;
	params.frameCount = WOLFX_LEVEL_OBJECTS::SPRITE_COUNT;
	params.startingFrame = WOLFX_LEVEL_OBJECTS::LIGHT;
	params.subAnimationFrameCount = 1;
	Sprite *test = spriteManager->AddSprite( "light", &params );

	// Load enemy sprite sheet
	MaterialDesc enemyMat;
	enemyMat.diffuseMaps[0] = WOLFX_RESOURCE::TEXTURE + "GuardSpriteSheet.png";
	enemyMat.shader = XOF_RESOURCE::SHADER + "DiffuseTexture";

	enemyMat.gpuStateName = "guardGPUState";
	enemyMat.gpuState.AddToDisabledFlags( XOF_GPU_STATE_FLAGS::CULLING );

	materialManager->AddMaterial( "guardMaterial", enemyMat );

	SpriteAnimationParams guardParams;
	guardParams.frameCount = 14;
	guardParams.startingFrame = 0;
	guardParams.subAnimationFrameCount = 14;
	spriteManager->AddSprite( "tempGuardSprite", &guardParams );

	// Setup temp door model
	meshManager = MeshManager::Get();
	MeshDesc tempDoorMesh;
	tempDoorMesh.type = MeshDesc::MESH_TYPE::GEO_PRIM_CUBE;
	tempDoorMesh.asCubeOrPlane[0] = 1; tempDoorMesh.asCubeOrPlane[1] = 1; tempDoorMesh.asCubeOrPlane[2] = 1;
	meshManager->AddMesh( "tempDoorMesh", tempDoorMesh );
	// Temp door material
	MaterialDesc tempDoorMat;
	tempDoorMat.diffuseMaps[0] = WOLFX_RESOURCE::TEXTURE + "DoorTestTexture.png";
	tempDoorMat.shader = XOF_RESOURCE::SHADER + "DiffuseTexture";
	// Have the door use the default GPU state settings (just leave this field blank)
	materialManager->AddMaterial( "tempDoorMaterial", tempDoorMat );

	// Load level
	LoadTempLevel( WOLFX_RESOURCE::LEVEL + "LevelMap.png" );
	// Set player camera
	gCamera.Setup( glm::vec3( 11.f, 0.4f, 26.f ), glm::vec3( 0.f, 0.f, 1.f ),
		static_cast<float>( mWindow->getSize().x ),  static_cast<float>( mWindow->getSize().y ),
		static_cast<float>( mWindow->getSize().x ) / static_cast<float>( mWindow->getSize().y ), 
		0.01f, 1000.f );

	gTempPlayer.reset( new Player( engineSystems, gCamera.GetPosition(), 
		nullptr, spriteManager->GetSprite( "weapon" ), 
		materialManager->GetMaterial("playerWeaponMaterial" ), &gCamera ) );
	//gGameObjects.push_back( (UniqueGameObjectPtr)( new Player( glm::vec3( 12.5f, 0.4f, 10.f ), nullptr, nullptr, nullptr, nullptr ) ) );

	// LOAD AUDIO
	AudioManager::Get()->AddSoundEffect( WOLFX_RESOURCE::AUDIO + "DOOROPENANDCLOSE.wav" );
}

void WolfX::HandleEvents() {
	sf::Event event;
	while( mWindow->pollEvent( event ) ) {
		if( event.type == sf::Event::Closed || mKeyboard.IsKeyDown( XOF_KEYBOARD::ESCAPE ) ) {
			mWindow->close();
		}

		if( event.type == sf::Event::Resized ) {
			mRenderer->Resize( event.size.width, event.size.height );
			gCamera.Resize( static_cast<float>( event.size.width ), static_cast<float>( event.size.height ) );
		}
	}
}

void WolfX::HandleInput() {
	// Movement
	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::A ) ) {
		gGameEvents.push( PlayerMovedEvent( glm::vec3( -0.001f, 0.f, 0.f ) ) );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::D ) ) {
		gGameEvents.push( PlayerMovedEvent( glm::vec3( 0.001f, 0.f, 0.f ) ) );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::W ) ) {
		gGameEvents.push( PlayerMovedEvent( glm::vec3( 0.f, 0.f, 0.001f ) ) );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::S ) ) {
		gGameEvents.push( PlayerMovedEvent( glm::vec3( 0.f, 0.f, -0.001f ) ) );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::Q ) ) {
		gCamera.Translate( 0.f, 0.001f, 0.f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::E ) ) {
		gCamera.Translate( 0.f, -0.001f, 0.f );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::R ) ) {
		///gTempBgm.Restart();
		gTempSoundFX.Play();
	}
	
	// Looking
	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_LEFT ) ) {
		//gCamera.Yaw( 0.001f );
		gGameEvents.push( PlayerMovedEvent( glm::vec3( 0.f, 0.f, 0.f ), 0.001f ) );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::ARROW_RIGHT ) ) {
		//gCamera.Yaw( -0.001f );
		gGameEvents.push( PlayerMovedEvent( glm::vec3( 0.f, 0.f, 0.f ), -0.001f ) );
	}

	// Action
	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::SPACE_BAR ) ) {
		gGameEvents.push( PlayerPressedActionKeyEvent( gCamera.GetPosition() ) );
	}

	if( mKeyboard.IsKeyDown( XOF_KEYBOARD::RIGHT_CONTROL ) ) {
		gGameEvents.push( PlayerPressedFireKeyEvent() );
	}
}

void WolfX::Render() {
	// Game objects submit their own render requests

	mEngine.renderer->SubmitRenderRequest( RenderRequest( XOF_RENDER_REQUEST_TYPE::RENDER_MESH,
		gTempLevel.tileMaterial, &gTempLevel.levelGeom, nullptr, 
		&gCamera, &gTempLevel.tileTransforms.at( 0 ) ) );

	mWindow->display();
}


#define BLACK 0x000000
#define GREY 0xA0A0A0A0
void LoadTempLevel( const std::string &fileName ) {
	gTempLevel.playerView = &gCamera;
	
	//gTempLevel.tiles.push_back( GeoPrimitiveGenerator::GenerateWolfXPlane( TILE_X, TILE_Z ) );
	gTempLevel.tileTransforms.push_back( Transform() );

	std::vector<Vertex>	vertexData;
	std::vector<U32>	indexData;

	Mesh tempWolfXLevel;

	// Set transforms based on pixels
	if( gTempLevel.levelMap.Load( fileName , true ) ) {
		I32 levelWidth = gTempLevel.levelMap.GetWidth();
		I32 levelHeight = gTempLevel.levelMap.GetHeight();

		float horizTextureOffset = 1.f / ATLAS_HORIZ_ELE_COUNT;
		float vertTextureOffset = 1.f / ATLAS_VERT_ELE_COUNT;

#pragma region LEVEL_GENERATION
		for( I32 i = 0; i < levelWidth; ++i ) {
			for( I32 j = 0; j < levelHeight; ++j )  {
				U32 pixel = gTempLevel.levelMap.GetPixel( i, j );
				pixel = ( pixel << 8 ) | pixel & 0xff;
				if( pixel != GREY ) {		
					U32 green = ( gTempLevel.levelMap.GetPixel( i, j ) & 0x00ff00 ) >> 8;
					float texStartX = static_cast<float>( horizTextureOffset * (green % ATLAS_HORIZ_ELE_COUNT) );
					float texEndX   = static_cast<float>( texStartX + horizTextureOffset );
					float texStartY = static_cast<float>( vertTextureOffset * (green / ATLAS_VERT_ELE_COUNT) );
					float texEndY   = static_cast<float>( texStartY + vertTextureOffset );

					// Floor
					indexData.push_back( vertexData.size() + 0 );
					indexData.push_back( vertexData.size() + 1 );
					indexData.push_back( vertexData.size() + 2 );

					indexData.push_back( vertexData.size() + 0 );
					indexData.push_back( vertexData.size() + 2 );
					indexData.push_back( vertexData.size() + 3 );

					vertexData.push_back( Vertex( glm::vec3( i * CELL_WIDTH, 0.f, j * CELL_HEIGHT ),					glm::vec2( texStartX,	texStartY ) ) );
					vertexData.push_back( Vertex( glm::vec3( ( i + 1 ) * CELL_WIDTH, 0.f, j * CELL_LENGTH ),			glm::vec2( texEndX,		texStartY ) ) );
					vertexData.push_back( Vertex( glm::vec3( ( i + 1 ) * CELL_WIDTH, 0.f, ( j + 1 ) * CELL_LENGTH ),	glm::vec2( texEndX,		texEndY ) ) );
					vertexData.push_back( Vertex( glm::vec3( i * CELL_WIDTH, 0.f, ( j + 1 ) * CELL_LENGTH ),			glm::vec2( texStartX,	texEndY ) ) );

					// Roof
					indexData.push_back( vertexData.size() + 2 );
					indexData.push_back( vertexData.size() + 1 );
					indexData.push_back( vertexData.size() + 0 );

					indexData.push_back( vertexData.size() + 3 );
					indexData.push_back( vertexData.size() + 2 );
					indexData.push_back( vertexData.size() + 0 );

					vertexData.push_back( Vertex( glm::vec3( i * CELL_WIDTH, CELL_HEIGHT, j * CELL_HEIGHT ),					glm::vec2( texStartX,	texStartY ) ) );
					vertexData.push_back( Vertex( glm::vec3( ( i + 1 ) * CELL_WIDTH, CELL_HEIGHT, j * CELL_LENGTH ),			glm::vec2( texEndX,		texStartY ) ) );
					vertexData.push_back( Vertex( glm::vec3( ( i + 1 ) * CELL_WIDTH, CELL_HEIGHT, ( j + 1 ) * CELL_LENGTH ),	glm::vec2( texEndX,		texEndY ) ) );
					vertexData.push_back( Vertex( glm::vec3( i * CELL_WIDTH, CELL_HEIGHT, ( j + 1 ) * CELL_LENGTH ),			glm::vec2( texStartX,	texEndY ) ) );

					// Walls
					U32 red = ( pixel & 0x0000ff );
					texStartX = static_cast<float>( horizTextureOffset * (red % ATLAS_HORIZ_ELE_COUNT) );
					texEndX   = static_cast<float>( texStartX + horizTextureOffset );
					texStartY = static_cast<float>( vertTextureOffset * (red / ATLAS_VERT_ELE_COUNT) );
					texEndY   = static_cast<float>( texStartY + vertTextureOffset );

					// cell above?
					U32 pix = gTempLevel.levelMap.GetPixel( i, j-1 );
					pix = (pix << 8) | pix & 0xff;
					if( pix == GREY ) {	
						indexData.push_back( vertexData.size() + 2 );
						indexData.push_back( vertexData.size() + 1 );
						indexData.push_back( vertexData.size() + 0 );

						indexData.push_back( vertexData.size() + 3 );
						indexData.push_back( vertexData.size() + 2 );
						indexData.push_back( vertexData.size() + 0 );

						vertexData.push_back( Vertex( glm::vec3( i * CELL_WIDTH,	0,				j * CELL_HEIGHT ), glm::vec2( texStartX,	texStartY ) ) );
						vertexData.push_back( Vertex( glm::vec3( (i+1) * CELL_WIDTH,	0,			j * CELL_LENGTH ), glm::vec2( texEndX,		texStartY ) ) );
						vertexData.push_back( Vertex( glm::vec3( (i+1) * CELL_WIDTH, CELL_HEIGHT,	j * CELL_HEIGHT ), glm::vec2( texEndX,		texEndY ) ) );
						vertexData.push_back( Vertex( glm::vec3( i * CELL_WIDTH, CELL_HEIGHT,		j * CELL_LENGTH ), glm::vec2( texStartX,	texEndY ) ) );
					}
		
					// cell below?
					pix = gTempLevel.levelMap.GetPixel( i, j+1 );
					pix = (pix << 8) | pix & 0xff;
					if( pix == GREY ) {							
						indexData.push_back( vertexData.size() + 0 );
						indexData.push_back( vertexData.size() + 1 );
						indexData.push_back( vertexData.size() + 2 );

						indexData.push_back( vertexData.size() + 0 );
						indexData.push_back( vertexData.size() + 2 );
						indexData.push_back( vertexData.size() + 3 );

						vertexData.push_back( Vertex( glm::vec3( i * CELL_WIDTH,	0,				(j+1) * CELL_HEIGHT ),	glm::vec2( texStartX,	texStartY ) ) );
						vertexData.push_back( Vertex( glm::vec3( (i+1) * CELL_WIDTH,	0,			(j+1) * CELL_LENGTH ),	glm::vec2( texEndX,		texStartY ) ) );
						vertexData.push_back( Vertex( glm::vec3( (i+1) * CELL_WIDTH, CELL_HEIGHT,	(j+1) * CELL_HEIGHT ),	glm::vec2( texEndX,		texEndY ) ) );
						vertexData.push_back( Vertex( glm::vec3( i * CELL_WIDTH, CELL_HEIGHT,		(j+1) * CELL_LENGTH ),	glm::vec2( texStartX,	texEndY ) ) );
					}

					// cell to the left?
					pix = gTempLevel.levelMap.GetPixel( i-1, j );
					pix = (pix << 8) | pix & 0xff;
					if( pix == GREY ) {					
						indexData.push_back( vertexData.size() + 0 );
						indexData.push_back( vertexData.size() + 1 );
						indexData.push_back( vertexData.size() + 2 );

						indexData.push_back( vertexData.size() + 0 );
						indexData.push_back( vertexData.size() + 2 );
						indexData.push_back( vertexData.size() + 3 );

						vertexData.push_back( Vertex( glm::vec3( i * CELL_WIDTH,	0,			j * CELL_HEIGHT ),		glm::vec2( texStartX,	texStartY ) ) );
						vertexData.push_back( Vertex( glm::vec3( i * CELL_WIDTH,	0,			(j+1) * CELL_LENGTH ),	glm::vec2( texEndX,		texStartY ) ) );
						vertexData.push_back( Vertex( glm::vec3( i * CELL_WIDTH, CELL_HEIGHT,	(j+1) * CELL_HEIGHT ),	glm::vec2( texEndX,		texEndY ) ) );
						vertexData.push_back( Vertex( glm::vec3( i * CELL_WIDTH, CELL_HEIGHT,	j * CELL_LENGTH ),		glm::vec2( texStartX,	texEndY ) ) );
					}

					// cell to the right?
					pix = gTempLevel.levelMap.GetPixel( i+1, j );
					pix = (pix << 8) | pix & 0xff;
					if( pix == GREY ) {				
						indexData.push_back( vertexData.size() + 2 );
						indexData.push_back( vertexData.size() + 1 );
						indexData.push_back( vertexData.size() + 0 );

						indexData.push_back( vertexData.size() + 3 );
						indexData.push_back( vertexData.size() + 2 );
						indexData.push_back( vertexData.size() + 0 );

						vertexData.push_back( Vertex( glm::vec3( (i+1) * CELL_WIDTH,	0,			j * CELL_HEIGHT ),		glm::vec2( texStartX,	texStartY ) ) );
						vertexData.push_back( Vertex( glm::vec3( (i+1) * CELL_WIDTH,	0,			(j+1) * CELL_LENGTH ),	glm::vec2( texEndX,		texStartY ) ) );
						vertexData.push_back( Vertex( glm::vec3( (i+1) * CELL_WIDTH, CELL_HEIGHT,	(j+1) * CELL_HEIGHT ),	glm::vec2( texEndX,		texEndY ) ) );
						vertexData.push_back( Vertex( glm::vec3( (i+1) * CELL_WIDTH, CELL_HEIGHT,	j * CELL_LENGTH ),		glm::vec2( texStartX,	texEndY ) ) );
					}

					// Game objects (Blue channel)
					Mover *door = nullptr;
					U32 blue = ( pixel & 0xff000000 );
					blue = blue >> 24;
					switch( blue / 32 ) {
						case 1: std::cout << "BONES"	<< std::endl; break;
						case 2: std::cout << "LIGHT"	<< std::endl; PlaceLight( i, j ); break;
						case 3: std::cout << "TURKEY"	<< std::endl; break;
						case 5: std::cout << "AMMO"		<< std::endl; break;
						case 6: std::cout << "DOOR"		<< std::endl; PlaceDoor( i, j, blue ); break;
						case 7: std::cout << "GUARD"	<< std::endl; PlaceGuard( i, j ); break;
					}
				} // if( !GREY )
			}
		}
#pragma endregion
	}

	gTempLevel.levelGeom.InitAsWolfXLevel( vertexData, indexData );

	MaterialDesc levelMat;
	levelMat.diffuseMaps[0] = WOLFX_RESOURCE::TEXTURE + "LevelAtlas.png";
	levelMat.shader = XOF_RESOURCE::SHADER + "DiffuseTexture";
	gTempLevel.tileMaterial = materialManager->AddMaterial( "levelMaterial", levelMat );
}


static void PlaceGuard( U32 xCoord, U32 yCoord ) {
	gGameObjects.push_back( (UniqueGameObjectPtr)( new Enemy( engineSystems, glm::vec3( xCoord, 1.f, yCoord ),
		nullptr, spriteManager->GetSprite( "tempGuardSprite" ), 
		materialManager->GetMaterial( "guardMaterial" ), &gCamera ) ) ); 
}

static void PlaceDoor( U32 xCoord, U32 yCoord, U32 doorCode ) {
	Mover *door = nullptr;

	// TODO: Replace this once the final door texture is done
	switch( doorCode % 32 ) {
		// left facing
		case 0: door = new Mover( engineSystems, Mover::MOVER_OBJECT_AXIS::Z, true, 0.9f, 1, 0.001f, 
		    				glm::vec3( xCoord, 0.f, yCoord ), meshManager->GetMesh( "tempDoorMesh" ), nullptr, 
		 					materialManager->GetMaterial( "tempDoorMaterial" ), &gCamera );
				door->Rotate( 0.f, -1.57f, -1.57f ); break;
		// front facing
		case 1: door = new Mover( engineSystems, Mover::MOVER_OBJECT_AXIS::X, true, 0.9f, 1, 0.001f, 
		    				glm::vec3( xCoord, 0.f, yCoord ), meshManager->GetMesh( "tempDoorMesh" ), nullptr, 
		 					materialManager->GetMaterial( "tempDoorMaterial" ), &gCamera );
				door->Rotate( -1.57f, 0.f, 0.f ); break;
	}

	if( door != nullptr ) {
		door->Scale( 1.f, 0.05f, 1.f );
		door->Translate( glm::vec3( 0.5f, 0.5f, 0.5f ) );
		gGameObjects.push_back( (UniqueGameObjectPtr)( door ) );
	}
}

static void PlaceLight( U32 xCoord, U32 yCoord ) {
	gGameObjects.push_back( (UniqueGameObjectPtr)( new BillBoard( engineSystems, glm::vec3( xCoord + 0.5f, 1.f, yCoord + 0.5f ),
												nullptr, spriteManager->GetSprite( "light" ), 
												materialManager->GetMaterial( "objectSheet" ), &gCamera ) ) );
}