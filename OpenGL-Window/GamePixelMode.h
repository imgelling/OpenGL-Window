#pragma once
#include <gl/GL.h>
#include "GameDefines.h"
#include "GameErrors.h"
#include "GameMath.h"
#include "GameTexture2D.h"
#include "GameEngine.h"

// | D3DFVF_TEX0 for tex coords WRONG!! need TEX1
#define PIXELMODEFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

namespace game
{
	extern GameError lastError;
	extern Engine* enginePointer;
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
		Texture2D _frameBuffer[2];
#if defined(GAME_SUPPORT_OPENGL) | defined(GAME_SUPPORT_ALL)
		uint32_t _compiledQuad;
#endif
#if defined(GAME_SUPPORT_DIRECTX9) | defined(GAME_SUPPORT_ALL)
		struct _CUSTOMVERTEX
		{
			FLOAT x, y, z, rhw;    
			DWORD color;    
			FLOAT u, v;
		};
		// modify pos values by -0.5f/width, -0.5f/height works dx9
		// opengl still broken (nvidia it works)
		_CUSTOMVERTEX OurVertices[6] =
		{
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 0.0f, 0.0f},
			{1280.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 1.0f, 0.0f},
			{0.0f, 720.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255) , 0.0f, 1.0f},

			{1280.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 1.0f, 0.0f},
			{1280.0f, 720.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 1.0f, 1.0f},
			{0.0f, 720.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255) , 0.0f, 1.0f}
		};
		LPDIRECT3DVERTEXBUFFER9 v_buffer;
		LPDIRECT3DDEVICE9 _d3d9Device;

#endif
		uint32_t* _video;
		Vector2i _bufferSize;
		Vector2i _windowSize;
		uint32_t _currentBuffer;
		void _UpdateFrameBuffer();
		void _ScaleQuadToWindow();
	};

	inline PixelModeFixed::PixelModeFixed()
	{
#if defined(GAME_SUPPORT_OPENGL) | defined(GAME_SUPPORT_ALL)
		_compiledQuad = 0;
#endif
		_video = nullptr;
		_currentBuffer = 0;
#if defined(GAME_SUPPORT_DIRECTX9) | defined(GAME_SUPPORT_ALL)
		v_buffer = nullptr;
		_d3d9Device = nullptr;
#endif
	}

	inline PixelModeFixed::~PixelModeFixed()
	{
		if (_video != nullptr) delete[] _video;
#if defined (GAME_SUPPORT_DIRECTX9) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			if (v_buffer)
			{
				v_buffer->Release();
			}
		}
#endif
		enginePointer->geUnLoadTexture(_frameBuffer[0]);
		enginePointer->geUnLoadTexture(_frameBuffer[1]);
	}

	inline bool PixelModeFixed::Initialize(const Vector2i& sizeOfScreen)
	{
		_bufferSize = sizeOfScreen;

		// Save window size
		_windowSize = enginePointer->geGetWindowSize();

		// Create video buffer
		_video = new uint32_t[_bufferSize.width * _bufferSize.height];
		if (_video == nullptr)
		{
			lastError = { GameErrors::GameRenderer, "Could not allocate RAM for PixelModeShaderless video buffer." };
			return false;
		}

		Clear(Colors::Black);

		// Create frame buffer textures
		for (uint32_t loop = 0; loop < 2; loop++)
		{
			_frameBuffer[loop].width = _bufferSize.width;
			_frameBuffer[loop].height = _bufferSize.height;
			_frameBuffer[loop].componentsPerPixel = 4;
			_frameBuffer[loop].filterType = game::TextureFilterType::Point;
			_frameBuffer[loop].isMipMapped = false;
			if (!enginePointer->geCreateTexture(_frameBuffer[loop]))
			{
				lastError = { GameErrors::GameRenderer, "Could not create textures for PixelModeShaderless frame buffers." };
				return false;
			}
		}

#if defined(GAME_SUPPORT_OPENGL) | defined(GAME_SUPPORT_ALL)
		// Generate the display list
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			_compiledQuad = glGenLists(1);
		}
#endif
#if defined (GAME_SUPPORT_DIRECTX9) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			_frameBuffer[0].textureInterface->GetDevice(&_d3d9Device);
			_d3d9Device->CreateVertexBuffer(6 * sizeof(_CUSTOMVERTEX), 0, PIXELMODEFVF, D3DPOOL_MANAGED, &v_buffer, NULL);
			if (v_buffer == nullptr)
			{
				lastError = { GameErrors::DirectXSpecific, "Could not create vertex buffer for PixelModeShaderless." };
				return false;
			}
		}
#endif

		// Scale the texture to window size
		_ScaleQuadToWindow();
		return true;
	}

	inline void PixelModeFixed::_UpdateFrameBuffer()
	{
#if defined(GAME_SUPPORT_OPENGL) | defined (GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			glBindTexture(GL_TEXTURE_2D, _frameBuffer[_currentBuffer].bind);

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _frameBuffer[_currentBuffer].width, _frameBuffer[_currentBuffer].height, GL_RGBA, game::systemInfo.gpuInfo.internalPixelType, (GLvoid*)_video);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
#endif
#if defined(GAME_SUPPORT_DIRECTX9) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			D3DLOCKED_RECT rect;
			unsigned char* test = (unsigned char*)_video;
			_frameBuffer[_currentBuffer].textureInterface->LockRect(0, &rect, 0, 0);
			unsigned char* dest = static_cast<unsigned char*>(rect.pBits);
			memcpy(dest, test, sizeof(unsigned char) * _frameBuffer[_currentBuffer].width * _frameBuffer[_currentBuffer].height * 4);
			_frameBuffer[_currentBuffer].textureInterface->UnlockRect(0);
		}
#endif
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
		
		// OpenGL Only
#if defined(GAME_SUPPORT_OPENGL) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			//positionOfScaledTexture.x -= _frameBuffer[_currentBuffer].oneOverWidth;
			//positionOfScaledTexture.y -= _frameBuffer[_currentBuffer].oneOverHeight;
			//sizeOfScaledTexture.x -= _frameBuffer[_currentBuffer].oneOverWidth;
			//sizeOfScaledTexture.y -= _frameBuffer[_currentBuffer].oneOverHeight;
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
#endif
#if defined(GAME_SUPPORT_DIRECTX9) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			VOID* pVoid = nullptr;    
			for (int i = 0; i < 6; i++)
			{
				OurVertices[i].x -= _frameBuffer[_currentBuffer].oneOverWidth;
				OurVertices[i].y -= _frameBuffer[_currentBuffer].oneOverHeight;
			}
			v_buffer->Lock(0, 0, (void**)&pVoid, 0);
			memcpy(pVoid, OurVertices, sizeof(OurVertices));    // copy vertices to the vertex buffer
			v_buffer->Unlock();
		}
#endif

	}

	inline void PixelModeFixed::Render()
	{
		Vector2i currentWindowSize = enginePointer->geGetWindowSize();

		// If window size has changed
		if ((currentWindowSize.width != _windowSize.width) || (currentWindowSize.height != _windowSize.height))
		{
			_windowSize = currentWindowSize;

			// Rescale the quad to fit new window size
			_ScaleQuadToWindow();
		}

		// Copy video buffer to gpu
		_UpdateFrameBuffer();

		// Draw the quad
		enginePointer->geEnable(GAME_TEXTURE_2D);
		enginePointer->geBindTexture(GAME_TEXTURE_2D, _frameBuffer[_currentBuffer]);
#if defined(GAME_SUPPORT_OPENGL) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			glCallList(_compiledQuad);
		}
#endif
#if defined(GAME_SUPPORT_DIRECTX9) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			DWORD oldFVF = 0;
			_d3d9Device->BeginScene();

			_d3d9Device->SetTexture(0, _frameBuffer[_currentBuffer].textureInterface);
			_d3d9Device->GetFVF(&oldFVF); // save current fvf
			_d3d9Device->SetFVF(PIXELMODEFVF);
			_d3d9Device->SetStreamSource(0, v_buffer, 0, sizeof(_CUSTOMVERTEX));
			_d3d9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
			_d3d9Device->SetFVF(oldFVF);  // reset to old fvf
			_d3d9Device->SetTexture(0, nullptr); // need to put back old texture if one is there

			_d3d9Device->EndScene();
		}
		
#endif
		enginePointer->geDisable(GAME_TEXTURE_2D);

		_currentBuffer++;
		if (_currentBuffer > 1) _currentBuffer = 0;

	}

	inline void PixelModeFixed::Clear(const Color &color)
	{
#if defined(GAME_SUPPORT_DIRECTX9) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			std::fill_n(_video, _bufferSize.width * _bufferSize.height, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b));
		}
#endif
#if defined(GAME_SUPPORT_OPENGL) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			std::fill_n(_video, _bufferSize.width * _bufferSize.height, color.packed);
		}
#endif
	}

	inline void PixelModeFixed::Pixel(const int32_t x, const int32_t y, const game::Color& color)
	{
#if defined(GAME_SUPPORT_DIRECTX9) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			_video[y * _bufferSize.width + x] = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
		}
#endif
#if defined(GAME_SUPPORT_OPENGL) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			_video[y * _bufferSize.width + x] = color.packed;
		}
#endif
	}

	inline void PixelModeFixed::PixelClip(const int32_t x, const int32_t y, const game::Color& color)
	{
		if (x < 0 || y < 0) return;
		if (x > _bufferSize.width-1 || y > _bufferSize.height - 1) return;
#if defined(GAME_SUPPORT_DIRECTX9) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			_video[y * _bufferSize.width + x] = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
		}
#endif
#if defined(GAME_SUPPORT_OPENGL) | defined(GAME_SUPPORT_ALL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			_video[y * _bufferSize.width + x] = color.packed;
		}
#endif
	}
}

#undef PIXELMODEFVF