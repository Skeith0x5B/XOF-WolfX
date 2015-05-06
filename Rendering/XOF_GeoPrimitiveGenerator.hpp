/*
===============================================================================

	XOF
	===
	File	:	XOF_GeoPrimitiveGenerator.hpp
	Desc	:	Class used to generate a handful of common geometric primitives.

===============================================================================
*/
#ifndef XOF_GEO_PRIMITIVE_GENERATOR_HPP
#define XOF_GEO_PRIMITIVE_GENERATOR_HPP


#include "XOF_Mesh.hpp"


class GeoPrimitiveGenerator {
public:
	friend class			Mesh;

	static UniqueMeshPtr	GenerateCube( I32 unitsAlongX, I32 unitsAlongY, I32 unitsAlongZ );
	static void				GenerateCube( I32 unitsAlongX, I32 unitsAlongY, I32 unitsAlongZ, Mesh& mesh );

	static UniqueMeshPtr	GeneratePlane( I32 unitsAlongX, I32 unitsAlongY, I32 unitsAlongZ, Mesh& mesh );

private:
};


#endif // XOF_GEO_PRIMITIVE_GENERATOR_HPP