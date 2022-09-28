#pragma once
#include <gl/GL.h>
#include "GameMath.h"
#include "GameErrors.h"

namespace game
{
	//  Needs to create a texture
	//  Needs to copy a texture to vram
	//		Need a texture property to pick mipmap, sampling, pow of 2 size, 
	//  Needs a color class
	//  Needs a working copy of frame
	//
	extern GameError _lastError;
	class PixelMode
	{
	public:
		PixelMode();
		~PixelMode();

		bool Initialize(const Vector2i &sizeOfScreen);
	private:
		Texture2dGL _frameBuffer[2];
		uint32_t _compiledQuad;
		uint32_t* _video;
		Vector2i _bufferSize;
	};

	inline PixelMode::PixelMode()
	{
		_compiledQuad = 0;
		_video = nullptr;
	}

	inline PixelMode::~PixelMode()
	{
		if (_video != nullptr) delete[] _video;
	}

	inline bool PixelMode::Initialize(const Vector2i& sizeOfScreen)
	{
		_bufferSize = sizeOfScreen;

		// Create video buffer
		_video = new uint32_t[_bufferSize.width * _bufferSize.height];
		if (_video == nullptr)
		{
			_lastError = { GameErrors::GameRenderer, "Could not allocate RAM for video buffer." };
			return false;
		}

		// Create frame buffer textures

		return true;
	}
}