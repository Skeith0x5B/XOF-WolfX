/*
===============================================================================

	XOF
	===
	File	:	XOF_GPUState.hpp
	Desc	:	Part of the material system, describes what GPU state settings
				to enfore when drawing a particular mesh/surface.

				GRAPHICS API AGNOSTIC. API SPECIFIC FLAGS ARRAY TO BE ADDED
				TO RENDERER CLASS/IMPLEMENTATION.

===============================================================================
*/
#ifndef XOF_GPU_STATE_HPP
#define XOF_GPU_STATE_HPP


#include "../Platform/XOF_Platform.hpp"


enum XOF_GPU_STATE_FLAGS {
	CW_WINDING	= 0,
	CCW_WINDING = 1,
	CULLING		= 2,
	CULL_BACK	= 3,
	CULL_FRONT	= 4,
	DEPTH_TEST	= 5,
	DEPTH_CLAMP = 6,
};


// highestEnabled/Disabled flag fields are automatically updated
class GPUState {
public:
	GPUState();
	GPUState( const GPUState& src );

	void AddToEnabledFlags( U8 flag );
	void AddToDisabledFlags( U8 flag );

	U16	GetEnabledFlags() const;
	U16	GetDisabledFlags() const;
	U8	GetHighestEnabledFlag() const;
	U8	GetHighestDisabledFlag() const;

	void Reset();

private:
	U16 mEnabledFlags;
	U16 mDisabledFlags;
	U8	mHighestEnabledFlag;
	U8	mHighestDisabledFlag;
};


using UniqueGPUStatePtr = std::unique_ptr<GPUState>;


#endif // XOF_GPU_STATE_HPP