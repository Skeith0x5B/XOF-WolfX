/*
===============================================================================

	XOF
	===
	File	:	XOF_Material.hpp
	Desc	:	Describe visual properties for some mesh.

===============================================================================
*/
#ifndef XOF_MATERIAL_HPP
#define XOF_MATERIAL_HPP


#include "../Platform/XOF_Platform.hpp"
#include "XOF_Texture.hpp"
#include "XOF_Shader.hpp"
class MaterialManager;


static const U8 XOF_MAX_TEXTURE_COUNT = 8;
static const U8 XOF_MAX_SHADER_COUNT = 2;


class Material {
public:
	friend class		MaterialManager;

						Material( const std::string &name );
						~Material();

	Texture*			GetTexture( XOF_TEXTURE_TYPE type, U32 index ) const;
	U8					GetTextureCount( XOF_TEXTURE_TYPE type ) const;
	Shader*				GetShader();

	// bool				AddShaderVariable();?
	// void				SetShaderVariables();?

private:
						// For testing purposes
	std::string			mName;

	Texture*			mTextures[XOF_TEXTURE_TYPE::TEXTURE_TYPE_COUNT][XOF_MAX_TEXTURE_COUNT];
	Shader*				mShader;
	U8					mTextureCounts[XOF_TEXTURE_TYPE::TEXTURE_TYPE_COUNT];
};


using UniqueMaterialPtr = std::unique_ptr<Material>;
using SharedMaterialPtr = std::shared_ptr<Material>;


#endif // XOF_MATERIAL_HPP