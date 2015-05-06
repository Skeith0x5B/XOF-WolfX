/*
===============================================================================

	XOF
	===
	File	:	XOF_Transform.hpp
	Desc	:	Represents a transformation, be it scale, rotation etc...
				NOTE: pos == translate?
				NOTE: Quarternions would be preferable for roatation to
						avoid gimbal lock...

				NOTE: Order of transformations?

===============================================================================
*/
#ifndef XOF_TRANSFORM_HPP
#define XOF_TRANSFORM_HPP


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


struct Transform {
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform() {
		this->translation = glm::vec3( 0.f, 0.f, 0.f );
		this->rotation = glm::vec3( 0.f, 0.f, 0.f );
		this->scale = glm::vec3( 1.f, 1.f, 1.f );
	}

	Transform( const glm::vec3 &translation, const glm::vec3 &rotation, 
		const glm::vec3 &scale = glm::vec3( 1.f, 1.f, 1.f ) ) {
		this->translation = translation;
		this->rotation = rotation;
		this->scale = scale;
	}

	// OpenGL tends to refer to world matrix as model matrix?
	inline glm::mat4 GetModelToWorldMatrix() const {
		glm::mat4 translationMatrix = glm::translate( translation );

		// NOTE: Quarternions would be preferable
		// Applied in inverse order (X, Y then Z)
		glm::mat4 rotationMatrix = glm::rotate( rotation.z, glm::vec3( 0.f, 0.f, 1.f ) ) *
				glm::rotate( rotation.y, glm::vec3( 0.f, 1.f, 0.f ) ) *
				glm::rotate( rotation.x, glm::vec3( 1.f, 0.f, 0.f ) );
		
		glm::mat4 scaleMatrix = glm::scale( scale );

		// Again, will be applied in inverse order (scale, rotation, translation)
		return translationMatrix * rotationMatrix * scaleMatrix;
	}
};


#endif // XOF_TRANSFORM_HPP