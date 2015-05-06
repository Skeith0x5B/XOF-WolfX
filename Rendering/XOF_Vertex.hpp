/*
===============================================================================

	XOF
	===
	File	:	XOF_Vertex.hpp
	Desc	:	Represents a vertex.

===============================================================================
*/
#ifndef XOF_VERTEX_HPP
#define XOF_VERTEX_HPP


#include <glm/glm.hpp>


struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;

	Vertex() {
		this->pos = glm::vec3( 0.f, 0.f, 0.f ); 
		this->normal = glm::vec3( 0.f, 0.f, 0.f );
		this->texCoords = glm::vec2( 0.f, 0.f );
		this->tangent = glm::vec3( 0.f, 0.f, 0.f );
	}

	Vertex(const glm::vec3& pos, glm::vec2& texCoords, 
		const glm::vec3 &normal = glm::vec3( 0.f, 0.f, 0.f ), 
		const glm::vec3 &tangent = glm::vec3( 0.f, 0.f, 0.f ) ) {
		this->pos = pos; 
		this->texCoords = texCoords;
		this->normal = normal;
		this->tangent = tangent;
	}
};


#endif // XOF_VERTEX_HPP