/*
===============================================================================

	XOF
	===
	File	:	XOF_Mesh.hpp
	Desc	:	Represents a mesh, be it from a file or manually created.

===============================================================================
*/
#ifndef XOF_MESH_HPP
#define XOF_MESH_HPP


#include "../Platform/XOF_Platform.hpp"
#include "XOF_Vertex.hpp"
#include <GL/glew.h>
#include <assimp/scene.h>
#include <vector>



class Mesh {
private:
	struct SubMesh;

public:
	// Give the geoprimgenerator easy access to the vertex and index data
	// for easy construction of primitives for use in the scene, editor brushes etc...
	friend	class				GeoPrimitiveGenerator;

								Mesh();
								Mesh( const std::string &fileName );
								~Mesh();

	bool						Load( const std::string &fileName );
	bool						IsLoaded() const;

	std::vector<Mesh::SubMesh>&	GetSubMeshData() const;
	U32							GetSubMeshCount() const;

								// TEMP
	void						InitAsWolfXLevel( const std::vector<Vertex>& vertexData,
									const std::vector<U32>& indexData );

private:
	// A whole mesh is built up as a collection of submeshes
	struct SubMesh {
				SubMesh();
				~SubMesh();

		GLuint  VB;
		GLuint  IB;
		size_t	indexCount;
		U8		materialIndex;

		void	Init( const std::vector<Vertex> &vertexData, 
					const std::vector<U32> &indexData );
	};

	enum XOF_MESH_BUFFER {
		POSITION_VB = 0,
		NORMAL_VB,
		TEXCOORD_VB,
		INDEX_VB,
		MESH_BUFFER_COUNT,
	};

	bool						mIsLoaded;
	std::vector<Mesh::SubMesh>	mSubMeshes;

	bool						InitMaterials( const aiScene *scene );
};


using UniqueMeshPtr = std::unique_ptr<Mesh>;


#endif // XOF_MESH_HPP