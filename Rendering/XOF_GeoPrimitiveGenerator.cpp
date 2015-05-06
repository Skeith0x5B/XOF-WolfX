/*
===============================================================================

	XOF
	===
	File	:	XOF_GeoPrimitiveGenerator.cpp
	Desc	:	Class used to generate a handful of common geometric primitives.

===============================================================================
*/
#include "XOF_GeoPrimitiveGenerator.hpp"


// What about texture coordinates?
UniqueMeshPtr GeoPrimitiveGenerator::GenerateCube( I32 unitsAlongX, I32 unitsAlongY, I32 unitsAlongZ ) {
	UniqueMeshPtr ptr( new Mesh() );
	GenerateCube( unitsAlongX, unitsAlongY, unitsAlongZ, *ptr );
	return ptr;
}

void GeoPrimitiveGenerator::GenerateCube( I32 unitsAlongX, I32 unitsAlongY, I32 unitsAlongZ, Mesh& mesh ) {
	// Add the 8 vertices for a box
	std::vector<Vertex> tempVertexData;
	std::vector<U32> tempIndexData;

	// NOTE: Temporarily having all the normals for these vertices point upward so they can interact with the lights
	tempVertexData.push_back( Vertex( glm::vec3( -unitsAlongX / 2.f, unitsAlongY / 2.f, -unitsAlongZ / 2.f ), glm::vec2( 0.f, 1.f ), glm::vec3( 0.f, 1.f, 0.f ) ) );
	tempVertexData.push_back( Vertex( glm::vec3(  unitsAlongX / 2.f, unitsAlongY / 2.f, -unitsAlongZ / 2.f ), glm::vec2( 1.f, 1.f ), glm::vec3( 0.f, 1.f, 0.f ) ) );
	tempVertexData.push_back( Vertex( glm::vec3(  unitsAlongX / 2.f, unitsAlongY / 2.f,  unitsAlongZ / 2.f ), glm::vec2( 1.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) ) );
	tempVertexData.push_back( Vertex( glm::vec3( -unitsAlongX / 2.f, unitsAlongY / 2.f,  unitsAlongZ / 2.f ), glm::vec2( 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) ) );

	tempVertexData.push_back( Vertex( glm::vec3( -unitsAlongX / 2.f, -unitsAlongY / 2.f, -unitsAlongZ / 2.f ), glm::vec2( 0.f, 1.f ), glm::vec3( 0.f, 1.f, 0.f ) ) );
	tempVertexData.push_back( Vertex( glm::vec3(  unitsAlongX / 2.f, -unitsAlongY / 2.f, -unitsAlongZ / 2.f ), glm::vec2( 1.f, 1.f ), glm::vec3( 0.f, 1.f, 0.f ) ) );
	tempVertexData.push_back( Vertex( glm::vec3(  unitsAlongX / 2.f, -unitsAlongY / 2.f,  unitsAlongZ / 2.f ), glm::vec2( 1.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) ) );
	tempVertexData.push_back( Vertex( glm::vec3( -unitsAlongX / 2.f, -unitsAlongY / 2.f,  unitsAlongZ / 2.f ), glm::vec2( 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) ) );

	// top
	tempIndexData.push_back( 0 ); tempIndexData.push_back( 1 ); tempIndexData.push_back( 2 );
	tempIndexData.push_back( 0 ); tempIndexData.push_back( 2 ); tempIndexData.push_back( 3 );

	// front
	tempIndexData.push_back( 3 ); tempIndexData.push_back( 2 ); tempIndexData.push_back( 6 );
	tempIndexData.push_back( 3 ); tempIndexData.push_back( 6 ); tempIndexData.push_back( 7 );

	// right
	tempIndexData.push_back( 2 ); tempIndexData.push_back( 1 ); tempIndexData.push_back( 5 );
	tempIndexData.push_back( 2 ); tempIndexData.push_back( 5 ); tempIndexData.push_back( 6 );

	// left
	tempIndexData.push_back( 0 ); tempIndexData.push_back( 3 ); tempIndexData.push_back( 7 );
	tempIndexData.push_back( 0 ); tempIndexData.push_back( 7 ); tempIndexData.push_back( 4 );

	// back
	tempIndexData.push_back( 1 ); tempIndexData.push_back( 0 ); tempIndexData.push_back( 4 );
	tempIndexData.push_back( 1 ); tempIndexData.push_back( 4 ); tempIndexData.push_back( 5 );

	// bot
	tempIndexData.push_back( 5 ); tempIndexData.push_back( 4 ); tempIndexData.push_back( 7 );
	tempIndexData.push_back( 5 ); tempIndexData.push_back( 7 ); tempIndexData.push_back( 6 );

	mesh.mSubMeshes.resize( 1 );
	mesh.mSubMeshes[0].Init( tempVertexData, tempIndexData );	

	// Material
	mesh.mSubMeshes[0].materialIndex = 0;

	mesh.mIsLoaded = true;
}

UniqueMeshPtr GeoPrimitiveGenerator::GeneratePlane( I32 unitsAlongX, I32 unitsAlongY, I32 unitsAlongZ, Mesh& mesh ) {
	//UniqueMeshPtr meshPtr( new Mesh() );

	std::vector<Vertex> tempVertexData; tempVertexData.resize( 4 );
	std::vector<U32> tempIndexData;

	// Verts
	tempVertexData[0] = Vertex( glm::vec3( -0.5f,					0,						0.f ), glm::vec2( 0.f, 1.f ) );
	tempVertexData[1] = Vertex( glm::vec3( tempVertexData[0].pos.x + 1,	0,						0.f ), glm::vec2( 1.f, 1.f ) );
	tempVertexData[2] = Vertex( glm::vec3( tempVertexData[0].pos.x + 1,	tempVertexData[0].pos.y - 1,	0.f ), glm::vec2( 1.f, 0.f ) );
	tempVertexData[3] = Vertex( glm::vec3( -0.5f,					tempVertexData[0].pos.y - 1,	0.f ), glm::vec2( 0.f, 0.f ) );

	//tempVertexData.push_back( Vertex( glm::vec3( 0,				  0,				0				), glm::vec2( 0.f, 1.f ), glm::vec3( 0.f, 1.f, 0.f ) ) );
	//tempVertexData.push_back( Vertex( glm::vec3( 0 + unitsAlongX, 0,				0				), glm::vec2( 1.f, 1.f ), glm::vec3( 0.f, 1.f, 0.f ) ) );
	//tempVertexData.push_back( Vertex( glm::vec3( 0 + unitsAlongX, 0 + unitsAlongY,	0 + unitsAlongZ ), glm::vec2( 1.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) ) );
	//tempVertexData.push_back( Vertex( glm::vec3( 0,				  0 + unitsAlongY,	0 + unitsAlongZ ), glm::vec2( 0.f, 0.f ), glm::vec3( 0.f, 1.f, 0.f ) ) );

	// Indexing
	tempIndexData.push_back( 0 ); tempIndexData.push_back( 1 ); tempIndexData.push_back( 2 );
	tempIndexData.push_back( 0 ); tempIndexData.push_back( 2 ); tempIndexData.push_back( 3 );

	mesh.mSubMeshes.resize( 1 );
	mesh.mSubMeshes[0].Init( tempVertexData, tempIndexData );	

	// Material
	mesh.mSubMeshes[0].materialIndex = 0;

	mesh.mIsLoaded= true;
	return nullptr;
}