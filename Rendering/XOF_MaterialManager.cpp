/*
===============================================================================

	XOF
	===
	File	:	XOF_MaterialManager.cpp
	Desc	:	Manage the matrials used by objects in the game.

===============================================================================
*/
#include "XOF_MaterialManager.hpp"
#include <iostream>


MaterialManager* MaterialManager::Get() {
	static MaterialManager singletonInstance;
	return &singletonInstance;
}

bool MaterialManager::StartUp() {
	MaterialDesc defaultMaterialDesc;
	defaultMaterialDesc.diffuseMaps[0] = XOF_RESOURCE::TEXTURE + "DEFAULT_TEXTURE.png";
	defaultMaterialDesc.shader = XOF_RESOURCE::SHADER + "DiffuseTexture";
	
	if( AddMaterial( "DEFAULT_MATERIAL", defaultMaterialDesc ) == nullptr ) {
		std::cerr << "MaterialManager::StartUp() - Default material creation failed" << std::endl;
		return false;
	}

	return true;
}
	
void MaterialManager::ShutDown() {
	mMaterials.clear();
	mTextures.clear();
	mShaders.clear();
}

Material* MaterialManager::AddMaterial( const std::string &materialName, const MaterialDesc &matDesc ) {
	Material *materialPtr = GetMaterial( materialName );
	if( !materialPtr ) {
		// Setup material based on the description
		Material material( materialName );
		
		// Set diffuse textures
		U32 i = 0;
		Texture *texturePtr = nullptr;
		while( matDesc.diffuseMaps[i].length() > 0 ) {
			if( ( texturePtr = ResolveTexture( matDesc.diffuseMaps[i] ) ) != nullptr ) {
				material.mTextures[XOF_TEXTURE_TYPE::DIFFUSE][i] = texturePtr;
				++material.mTextureCounts[XOF_TEXTURE_TYPE::DIFFUSE];
			}
			++i;
		}

		// Set normal maps
		i = 0;
		while( matDesc.normalMaps[i].length() > 0 ) {
			if( ( texturePtr = ResolveTexture( matDesc.normalMaps[i] ) ) != nullptr ) {
				material.mTextures[XOF_TEXTURE_TYPE::NORMAL][i] = texturePtr;
				++material.mTextureCounts[XOF_TEXTURE_TYPE::NORMAL];
			}
			++i;
		}

		// Set shader (vertex/fragment shader pair)
		Shader *shaderPtr = nullptr;
		if( ( ( shaderPtr = ResolveShader( matDesc.shader ) ) != nullptr ) ) {
			material.mShader = shaderPtr;
		}

		// Add to manager
		auto itr = mMaterials.emplace( materialName, material );//std::move( material ) ); // will need Material(const Material&& m) {}
		if( itr.first == mMaterials.end() ) {
			std::cerr << "MaterialManager::AddMaterial() - Failed to add material: " <<
				materialName << std::endl;
			return nullptr;
		}
		materialPtr = &itr.first->second;
	}
	return materialPtr;
}

Material* MaterialManager::GetMaterial( const std::string &materialName ) const {
	Material *material = nullptr;
	auto itr = mMaterials.find( materialName );
	if( itr != mMaterials.end() ) {
		material = const_cast<Material*>( &itr->second );
	}
	return material;
}

// TODO: Replace emplace with insert?
Texture* MaterialManager::ResolveTexture( const std::string &textureName ) {
 	UniqueTexturePtr texture( new Texture( textureName ) );

	Texture *texturePtr = nullptr;
	if( texture->IsLoaded() ) {
		auto itr = mTextures.emplace( textureName, std::move( texture ) );
		if( itr.first == mTextures.end() ) {
			std::cerr << "MaterialManager::ResolveTexture() - Failed to add texture: " <<
				textureName << std::endl;
			return nullptr;
		}
		texturePtr = itr.first->second.get();
	} else {
		std::cerr << "MaterialManager::ResolveTexture() - Failed to load texture : " << 
			textureName << std::endl;
	}

	return texturePtr;
}

Shader* MaterialManager::ResolveShader( const std::string &shaderName ) {
	UniqueShaderPtr shader( new Shader( shaderName ) );

	Shader *shaderPtr = nullptr;
	if( shader->IsLoaded() ) {
		auto itr = mShaders.emplace( shaderName, std::move( shader ) );
		if( itr.first == mShaders.end() ) {
			std::cerr << "MaterialManager::ResolveShader() - Failed to add Shader: " <<
				shaderName << std::endl;
			return nullptr;
		}
		shaderPtr = itr.first->second.get();
	} else {
		std::cerr << "MaterialManager::ResolveShader() - Failed to load Shader : " << 
			shaderName << std::endl;
	}

	return shaderPtr;
}