/*
===============================================================================

	XOF
	===
	File	:	XOF_MaterialManager.hpp
	Desc	:	Manage the matrials used by objects in the game.

===============================================================================
*/
#ifndef XOF_MATERIAL_MANAGER_HPP
#define XOF_MATERIAL_MANAGER_HPP


#include "../Platform/XOF_Platform.hpp"
#include "XOF_Material.hpp"
#include "XOF_Texture.hpp"
#include "XOF_Shader.hpp"
#include <unordered_map>


struct MaterialDesc {
	std::string diffuseMaps[XOF_MAX_TEXTURE_COUNT];
	std::string normalMaps[XOF_MAX_TEXTURE_COUNT];
	std::string shader;
	// Device config (winding order etc)
	std::string	gpuStateName;
	GPUState	gpuState;
};


class MaterialManager {
public:
	static MaterialManager*	Get();

	bool					StartUp();
	void					ShutDown();

	Material*				AddMaterial( const std::string &materialName, const MaterialDesc &matDesc );
	Material*				GetMaterial( const std::string &materialName ) const;

private:
	// TODO: Replace string with hash value? Container already hashes?
	std::unordered_map<std::string, Material>			mMaterials;
	std::unordered_map<std::string, UniqueTexturePtr>	mTextures;
	std::unordered_map<std::string, UniqueShaderPtr>	mShaders;
	std::unordered_map<std::string, UniqueGPUStatePtr>	mGPUStates;

							// Load the textures and shaders mentioned in a given MaterialDesc
	Texture*				ResolveTexture( const std::string &textureName );
	Shader*					ResolveShader( const std::string &shaderName );
	GPUState*				ResolveGPUState( const std::string& gpuStateName, const GPUState &gpuState );
};


#endif // XOF_MATERIAL_MANAGER_HPP