#pragma once
#include <gl/GL.h>
#include "GameMath.h"
#include "GameErrors.h"
#include "GameTexture2D.h"

namespace game
{
	extern GameError lastError;
	class PixelModeFixed
	{
	public:
		PixelModeFixed();
		~PixelModeFixed();

		bool Initialize(const Vector2i& sizeOfScreen);
		void Render();
		void Clear(const Color &color);
		void Pixel(const int32_t x, const int32_t y, const game::Color& color);
		void PixelClip(const int32_t x, const int32_t y, const game::Color& color);
	private:
		Texture2dGL _frameBuffer[2];
		uint32_t _compiledQuad;
		uint32_t* _video;
		Vector2i _bufferSize;
		Vector2i _windowSize;
		uint32_t _currentBuffer; // gl only
		void _UpdateFrameBuffer();
		void _ScaleQuadToWindow();
	};

	inline PixelModeFixed::PixelModeFixed()
	{
		// gl only
		_compiledQuad = 0;
		_video = nullptr;
		_currentBuffer = 0;
	}

	inline PixelModeFixed::~PixelModeFixed()
	{
		if (_video != nullptr) delete[] _video;
	}

	inline bool PixelModeFixed::Initialize(const Vector2i& sizeOfScreen)
	{
		_bufferSize = sizeOfScreen;

		// Save window size
		_windowSize = enginePointer->GetWindowSize();

		// Create video buffer
		_video = new uint32_t[_bufferSize.width * _bufferSize.height];
		if (_video == nullptr)
		{
			lastError = { GameErrors::GameRenderer, "Could not allocate RAM for PixelModeShaderless video buffer." };
			return false;
		}

		// gl only
		// Create frame buffer textures
		for (uint32_t loop = 0; loop < 2; loop++)
		{
			_frameBuffer[loop].width = _bufferSize.width;
			_frameBuffer[loop].height = _bufferSize.height;
			_frameBuffer[loop].componentsPerPixel = 4;
			_frameBuffer[loop].filterType = game::TextureFilterType::Point;
			_frameBuffer[loop].isMipMapped = false;
			if (!enginePointer->CreateTexture(_frameBuffer[loop]))
			{
				lastError = { GameErrors::GameRenderer, "Could not create textures for PixelModeShaderless frame buffers." };
				return false;
			}
		}

		// gl only
		// Generate the display list
		_compiledQuad = glGenLists(1);

		// Scale the texture to window size
		_ScaleQuadToWindow();

		return true;
	}

	inline void PixelModeFixed::_UpdateFrameBuffer()
	{
		// gl only
		// needs to double buffer
		glBindTexture(GL_TEXTURE_2D, _frameBuffer[_currentBuffer].bind);

		// gl only
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _frameBuffer[_currentBuffer].width, _frameBuffer[_currentBuffer].height, GL_RGBA, game::systemInfo.gpuInfo.internalPixelType, (GLvoid*)_video);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		//// Swap texture buffer to draw to
		//_currentBuffer++;
		//if (_currentBuffer > 1) _currentBuffer = 0;
	}

	inline void PixelModeFixed::_ScaleQuadToWindow()
	{

		game::Vector2f positionOfScaledTexture;
		game::Vector2f scale;
		game::Vector2f sizeOfScaledTexture;
		float_t tempScale = 0.0f;
		if (_windowSize.height < _windowSize.width)
		{
			scale.y = (float_t)_windowSize.height * _frameBuffer[_currentBuffer].oneOverHeight;
			tempScale = (float_t)_windowSize.width * _frameBuffer[_currentBuffer].oneOverWidth;
			if (tempScale > scale.y)
			{
				scale.x = scale.y;
			}
			else
			{
				scale.x = scale.y = tempScale;
				positionOfScaledTexture.y = ((_windowSize.height >> 1) - ((float_t)_frameBuffer[_currentBuffer].height * scale.y / 2.0f));
			}
			positionOfScaledTexture.x = ((_windowSize.width >> 1) - ((float_t)_frameBuffer[_currentBuffer].width * scale.x / 2.0f));
		}
		else if (_windowSize.height > _windowSize.width)
		{
			scale.x = (float_t)_windowSize.width * _frameBuffer[_currentBuffer].oneOverWidth;
			scale.y = scale.x;
			positionOfScaledTexture.y = ((_windowSize.height >> 1) - ((float_t)_frameBuffer[_currentBuffer].height * scale.y / 2.0f));
		}
		else
		{
			scale = { 1.0f, 1.0f };
		}

		// Set the size of the scaled texture
		sizeOfScaledTexture.width = positionOfScaledTexture.x + (_frameBuffer[_currentBuffer].width * scale.x);
		sizeOfScaledTexture.height = positionOfScaledTexture.y + (_frameBuffer[_currentBuffer].height * scale.y);

		// Homoginize the scaled rect to -1 to 1 range using
		// position.x = position.x * 2.0 / width - 1.0
		// position.y = position.y * 2.0 / height - 1.0;
		positionOfScaledTexture.x = (positionOfScaledTexture.x * 2.0f / (float_t)_windowSize.width) - 1.0f;
		positionOfScaledTexture.y = (positionOfScaledTexture.y * 2.0f / (float_t)_windowSize.height) - 1.0f;
		sizeOfScaledTexture.width = ((float_t)sizeOfScaledTexture.width * 2.0f / (float_t)_windowSize.width) - 1.0f;
		sizeOfScaledTexture.height = ((float_t)sizeOfScaledTexture.height * 2.0f / (float_t)_windowSize.height) - 1.0f;
		
		// gl only
		glNewList(_compiledQuad, GL_COMPILE);
		{
			glBegin(GL_QUADS);
			//bl
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0, 0);
			glVertex2f(positionOfScaledTexture.x, -sizeOfScaledTexture.height);
			//br
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(sizeOfScaledTexture.width, -sizeOfScaledTexture.height);
			//tr
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(1, 1);
			glVertex2f(sizeOfScaledTexture.width, -positionOfScaledTexture.y);
			// tl
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0, 1);
			glVertex2f(positionOfScaledTexture.x, -positionOfScaledTexture.y);

			glEnd();
		}
		glEndList();
	}

	inline void PixelModeFixed::Render()
	{
		Vector2i currentWindowSize;

		currentWindowSize = enginePointer->GetWindowSize();

		// If window size has changed
		if ((currentWindowSize.width != _windowSize.width) || (currentWindowSize.height != _windowSize.height))
		{
			_windowSize = currentWindowSize;

			// Rescale the quad to fit new window size
			_ScaleQuadToWindow();
		}

		// Copy video buffer to gpu
		_UpdateFrameBuffer();

		// gl only
		// Draw the quad
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _frameBuffer[_currentBuffer].bind);
		glCallList(_compiledQuad);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		_currentBuffer++;
		if (_currentBuffer > 1) _currentBuffer = 0;

	}

	inline void PixelModeFixed::Clear(const Color &color)
	{
		std::fill_n(_video, _bufferSize.width * _bufferSize.height, color.packed);
	}

	inline void PixelModeFixed::Pixel(const int32_t x, const int32_t y, const game::Color& color)
	{
		_video[y * _bufferSize.width + x] = color.packed;
	}

	inline void PixelModeFixed::PixelClip(const int32_t x, const int32_t y, const game::Color& color)
	{
		if (x < 0) return;
		if (y < 0) return;
		if (x > _bufferSize.width-1) return;
		if (y > _bufferSize.height-1) return;
		_video[y * _bufferSize.width + x] = color.packed;
	}
}