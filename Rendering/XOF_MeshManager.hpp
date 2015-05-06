/*
===============================================================================

	XOF
	===
	File	:	XOF_MeshManager.hpp
	Desc	:	Manage the meshes used by the game objects.

===============================================================================
*/
#ifndef XOF_MESH_MANAGER_HPP
#define XOF_MESH_MANAGER_HPP


#include "../Platform/XOF_Platform.hpp"
#include "XOF_Mesh.hpp"
#include <unordered_map>


struct MeshDesc {
	enum MESH_TYPE {
		OBJ,
		GEO_PRIM_CUBE,
		GEO_PRIM_PLANE,
	};

	U32 type;

	union {
		//U8 *asMeshFile;
		U32 asCubeOrPlane[3];
		// asSphere
		// asCylinder
		// etc...
	};
};


class MeshManager {
public:
	static MeshManager*	Get();

	bool					StartUp();
	void					ShutDown();

	Mesh*					AddMesh( const std::string &meshName, const MeshDesc& meshDesc );
	Mesh*					GetMesh( const std::string &meshName ) const;

private:
	// TODO: Replace string with hash value? Container already has
	std::unordered_map<std::string, UniqueMeshPtr>	mMeshes;
};


#endif // XOF_MESH_MANAGER_HPP