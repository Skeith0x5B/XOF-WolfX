/*
===============================================================================

	XOF
	===
	File	:	XOF_GPUState.cpp
	Desc	:	Part of the material system, describes what GPU state settings
				to enfore when drawing a particular mesh/surface.

===============================================================================
*/
#include "XOF_GPUState.hpp"


GPUState::GPUState() { 
	Reset();
}

GPUState::GPUState( const GPUState& src ) {
	mEnabledFlags			= src.mEnabledFlags;
	mHighestEnabledFlag		= src.mHighestEnabledFlag;
	mDisabledFlags			= src.mDisabledFlags;
	mHighestDisabledFlag	= src.mHighestDisabledFlag;
}

void GPUState::AddToEnabledFlags( U8 flag ) {
	mEnabledFlags |= ( 1 << flag );
	if( flag > mHighestEnabledFlag ) {
		mHighestEnabledFlag = flag;
	}
}

void GPUState::AddToDisabledFlags( U8 flag ) {
	mDisabledFlags |= ( 1 << flag );
	if( flag > mHighestDisabledFlag ) {
		mHighestDisabledFlag = flag;
	}
}

U16	GPUState::GetEnabledFlags() const {
	return mEnabledFlags;
}

U16	GPUState::GetDisabledFlags() const {
	return mDisabledFlags;
}

U8 GPUState::GetHighestEnabledFlag() const {
	return mHighestEnabledFlag;
}

U8 GPUState::GetHighestDisabledFlag() const {
	return mHighestDisabledFlag;
}

void GPUState::Reset() {
	mEnabledFlags = mDisabledFlags = 0;
	mHighestEnabledFlag = mHighestDisabledFlag = 0;
}