/*
===============================================================================

	XOF
	===
	File	:	XOF_MaterialManager.hpp
	Desc	:	Manage the matrials used by objects in the game.

===============================================================================
*/
#include "XOF_Material.hpp"


Material::Material( const std::string &name ) { 
	mName = name;
	mTextureCounts[XOF_TEXTURE_TYPE::DIFFUSE] = 0;
	mTextureCounts[XOF_TEXTURE_TYPE::NORMAL] = 0;
}

Material::~Material() {
}

Texture* Material::GetTexture( XOF_TEXTURE_TYPE type, U32 index ) const {
	return mTextures[type][index];
}

U8 Material::GetTextureCount( XOF_TEXTURE_TYPE type ) const {
	return mTextureCounts[type];
}

Shader* Material::GetShader() {
	return mShader;
}