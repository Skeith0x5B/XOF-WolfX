/*
===============================================================================

	XOF
	===
	File	:	XOF_MeshManager.cpp
	Desc	:	Manage the meshes used by the game objects.

===============================================================================
*/
#include "XOF_MeshManager.hpp"
#include "XOF_GeoPrimitiveGenerator.hpp"


MeshManager* MeshManager::Get() {
	static MeshManager singletonInstance;
	return &singletonInstance;
}

bool MeshManager::StartUp() {
	return true;
}
	
void MeshManager::ShutDown() {
	mMeshes.clear();
}

Mesh* MeshManager::AddMesh( const std::string &meshName, const MeshDesc& meshDesc ) {
	Mesh *meshPtr = GetMesh( meshName );
	if( !meshPtr ) {
		// Setup a mesh
		UniqueMeshPtr mesh( new Mesh() );
		switch( meshDesc.type ) {
			case MeshDesc::MESH_TYPE::OBJ: mesh->Load( XOF_RESOURCE::MESH + "Booker_DeWitt.obj" ); break;
			// TODO: Add other primitive geometry types
			case MeshDesc::MESH_TYPE::GEO_PRIM_CUBE: GeoPrimitiveGenerator::GenerateCube(
														meshDesc.asCubeOrPlane[0], meshDesc.asCubeOrPlane[1], 
														meshDesc.asCubeOrPlane[2], *mesh ); break;
			case MeshDesc::MESH_TYPE::GEO_PRIM_PLANE: GeoPrimitiveGenerator::GeneratePlane(
														meshDesc.asCubeOrPlane[0], meshDesc.asCubeOrPlane[1], 
														meshDesc.asCubeOrPlane[2], *mesh ); break;
		}

		if( !mesh->IsLoaded() ) {
			std::cerr << "MeshManager::AddMesh() - Failed to load mesh: " << meshName << std::endl;
		}

		// Add to manager
		auto itr = mMeshes.emplace( meshName, std::move( mesh ) );
		if( itr.first == mMeshes.end() ) {
			std::cerr << "MeshManager::AddMesh() - Failed to add mesh: " << meshName << std::endl;
			return nullptr;
		}
		meshPtr = itr.first->second.get();
	}
	return meshPtr;
}

Mesh* MeshManager::GetMesh( const std::string &meshName ) const {
	Mesh *mesh = nullptr;
	auto itr = mMeshes.find( meshName );
	if( itr != mMeshes.end() ) {
		mesh = itr->second.get();
	}
	return mesh;
}