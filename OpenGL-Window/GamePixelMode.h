#pragma once
#if defined(GAME_OPENGL)
#include <gl/GL.h>
#endif
#if defined(GAME_DIRECTX9)
#include <d3d9.h>
#endif
#include "GameDefines.h"
#include "GameErrors.h"
#include "GameEngine.h"
#include "GameMath.h"
#include "GameTexture2D.h"

namespace game
{
	extern GameError lastError;
	extern Engine* enginePointer;
	class PixelMode
	{
	public:
		PixelMode();
		~PixelMode();

		bool Initialize(const Vector2i& sizeOfScreen);
		void Render();
		void Clear(const Color &color);
		void Pixel(const int32_t x, const int32_t y, const game::Color& color);
		void PixelClip(const int32_t x, const int32_t y, const game::Color& color);
	private:
		Texture2D _frameBuffer[2];
#if defined(GAME_OPENGL)
		uint32_t _compiledQuad;
#endif
#if defined(GAME_DIRECTX9)
		struct _vertex
		{
			float_t x, y, z, rhw;    
			uint32_t color;    
			float_t u, v;
		};
		_vertex _triangleVertices[6] =
		{
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 0.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255) , 0.0f, 1.0f},

			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 1.0f, 1.0f},
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255) , 0.0f, 1.0f}
		};
		LPDIRECT3DVERTEXBUFFER9 _vertexBuffer;
		LPDIRECT3DDEVICE9 _d3d9Device;
#endif
		uint32_t* _video;
		Vector2i _bufferSize;
		Vector2i _windowSize;
		uint32_t _currentBuffer;
		void _UpdateFrameBuffer();
		void _ScaleQuadToWindow();
	};

	inline PixelMode::PixelMode()
	{
#if defined(GAME_OPENGL)
		_compiledQuad = 0;
#endif
		_video = nullptr;
		_currentBuffer = 0;
#if defined(GAME_DIRECTX9)
		_vertexBuffer = nullptr;
		_d3d9Device = nullptr;
#endif
	}

	inline PixelMode::~PixelMode()
	{
		if (_video != nullptr) delete[] _video;
#if defined (GAME_DIRECTX9)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			if (_vertexBuffer)
			{
				_vertexBuffer->Release();
				_vertexBuffer = nullptr;
			}
			if (_d3d9Device)
			{
				_d3d9Device->Release();
				_d3d9Device = nullptr;
			}
		}
#endif
		enginePointer->geUnLoadTexture(_frameBuffer[0]);
		enginePointer->geUnLoadTexture(_frameBuffer[1]);
	}

	inline bool PixelMode::Initialize(const Vector2i& sizeOfScreen)
	{
		_bufferSize = sizeOfScreen;

		// Save window size
		_windowSize = enginePointer->geGetWindowSize();

		// Create video buffer
		_video = new uint32_t[(uint32_t)_bufferSize.width * (uint32_t)_bufferSize.height];
		if (_video == nullptr)
		{
			lastError = { GameErrors::GameRenderer, "Could not allocate RAM for PixelMode video buffer." };
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
				lastError = { GameErrors::GameRenderer, "Could not create textures for PixelMode frame buffers." };
				return false;
			}
		}

#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			_compiledQuad = glGenLists(1);
		}
#endif
#if defined (GAME_DIRECTX9)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			_frameBuffer[0].textureInterface9->GetDevice(&_d3d9Device);
			_d3d9Device->CreateVertexBuffer(6 * sizeof(_vertex), 0, (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1), D3DPOOL_MANAGED, &_vertexBuffer, NULL);
			if (_vertexBuffer == nullptr)
			{
				lastError = { GameErrors::GameDirectX9Specific, "Could not create vertex buffer for PixelMode." };
				return false;
			}
		}
#endif

		// Scale the texture to window size
		_ScaleQuadToWindow();
		return true;
	}

	inline void PixelMode::_UpdateFrameBuffer()
	{
#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			glBindTexture(GL_TEXTURE_2D, _frameBuffer[_currentBuffer].bind);

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _frameBuffer[_currentBuffer].width, _frameBuffer[_currentBuffer].height, GL_RGBA, game::systemInfo.gpuInfo.internalPixelType, (GLvoid*)_video);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
#endif
#if defined(GAME_DIRECTX9)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			D3DLOCKED_RECT rect;
			unsigned char* test = (unsigned char*)_video;
			_frameBuffer[_currentBuffer].textureInterface9->LockRect(0, &rect, 0, 0);
			unsigned char* dest = static_cast<unsigned char*>(rect.pBits);
			memcpy(dest, test, sizeof(unsigned char) * _frameBuffer[_currentBuffer].width * _frameBuffer[_currentBuffer].height * 4);
			_frameBuffer[_currentBuffer].textureInterface9->UnlockRect(0);
		}
#endif
	}

	inline void PixelMode::_ScaleQuadToWindow()
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


#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			// Pixel offset fix
			positionOfScaledTexture.x -= _frameBuffer[_currentBuffer].oneOverWidth;
			positionOfScaledTexture.y -= _frameBuffer[_currentBuffer].oneOverHeight;
			sizeOfScaledTexture.width -= _frameBuffer[_currentBuffer].oneOverWidth;
			sizeOfScaledTexture.height -= _frameBuffer[_currentBuffer].oneOverHeight;

			// Homoginize the scaled rect to -1 to 1 range using
			positionOfScaledTexture.x = (positionOfScaledTexture.x * 2.0f / (float_t)_windowSize.width) - 1.0f;
			positionOfScaledTexture.y = (positionOfScaledTexture.y * 2.0f / (float_t)_windowSize.height) - 1.0f;
			sizeOfScaledTexture.width = ((float_t)sizeOfScaledTexture.width * 2.0f / (float_t)_windowSize.width) - 1.0f;
			sizeOfScaledTexture.height = ((float_t)sizeOfScaledTexture.height * 2.0f / (float_t)_windowSize.height) - 1.0f;

			glNewList(_compiledQuad, GL_COMPILE);
			{
				glBegin(GL_QUADS);
				//bl
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(0, 1);
				glVertex2f(positionOfScaledTexture.x, -sizeOfScaledTexture.height);
				//br
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(1, 1);
				glVertex2f(sizeOfScaledTexture.width, -sizeOfScaledTexture.height);
				//tr
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(sizeOfScaledTexture.width, -positionOfScaledTexture.y);
				// tl
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(0, 0);
				glVertex2f(positionOfScaledTexture.x, -positionOfScaledTexture.y);

				glEnd();
			}
			glEndList();
		}
#endif
#if defined(GAME_DIRECTX9)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			VOID* pVoid = nullptr;  

			// Pixel offset fix
			positionOfScaledTexture.x -= _frameBuffer[_currentBuffer].oneOverWidth;
			positionOfScaledTexture.y -= _frameBuffer[_currentBuffer].oneOverHeight;
			sizeOfScaledTexture.width -= _frameBuffer[_currentBuffer].oneOverWidth;
			sizeOfScaledTexture.height -= _frameBuffer[_currentBuffer].oneOverHeight;

			// tl
			_triangleVertices[0].x = positionOfScaledTexture.x;
			_triangleVertices[0].y = positionOfScaledTexture.y;
			// tr
			_triangleVertices[1].x = sizeOfScaledTexture.width;
			_triangleVertices[1].y = positionOfScaledTexture.y;
			// bl
			_triangleVertices[2].x = positionOfScaledTexture.x;
			_triangleVertices[2].y = sizeOfScaledTexture.height;

			// tr
			_triangleVertices[3].x = sizeOfScaledTexture.width;
			_triangleVertices[3].y = positionOfScaledTexture.y;
			// br
			_triangleVertices[4].x = sizeOfScaledTexture.width;
			_triangleVertices[4].y = sizeOfScaledTexture.height;
			// bl
			_triangleVertices[5].x = positionOfScaledTexture.x;
			_triangleVertices[5].y = sizeOfScaledTexture.height;

			// Copy vertices to the vertex buffer
			_vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
			memcpy(pVoid, _triangleVertices, sizeof(_triangleVertices));
			_vertexBuffer->Unlock();
		}
#endif

	}

	inline void PixelMode::Render()
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
#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			//enginePointer->geEnable(GAME_TEXTURE_2D);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _frameBuffer[_currentBuffer].bind);
			//enginePointer->geBindTexture(GAME_TEXTURE_2D, _frameBuffer[_currentBuffer]);
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				glDisable(0x809D); // 0x809D is GL_MULTISAMPLE
			}
			glCallList(_compiledQuad);
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				glEnable(0x809D);
			}
			//enginePointer->geDisable(GAME_TEXTURE_2D);
			glDisable(GL_TEXTURE_2D);
		}
#endif
#if defined(GAME_DIRECTX9)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			DWORD oldFVF = 0;
			IDirect3DBaseTexture9* activeTexture = 0;
			_d3d9Device->BeginScene();
			// Save current state
			_d3d9Device->GetFVF(&oldFVF); 
			_d3d9Device->GetTexture(0, &activeTexture);

			// Disable multisampling if enabled
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				_d3d9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
			}

			_d3d9Device->SetTexture(0, _frameBuffer[_currentBuffer].textureInterface9);
			_d3d9Device->SetFVF((D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1));
			_d3d9Device->SetStreamSource(0, _vertexBuffer, 0, sizeof(_vertex));
			_d3d9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

			// Restore previous state
			_d3d9Device->SetFVF(oldFVF);  
			_d3d9Device->SetTexture(0, activeTexture); 
			if (activeTexture)
			{
				activeTexture->Release();
			}

			// Renable multisampling if it was enabled
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				_d3d9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
			}

			_d3d9Device->EndScene();
		}
		
#endif

		_currentBuffer++;
		if (_currentBuffer > 1) _currentBuffer = 0;

	}

	inline void PixelMode::Clear(const Color &color)
	{
#if defined(GAME_DIRECTX9)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			std::fill_n(_video, _bufferSize.width * _bufferSize.height, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b));
		}
#endif
#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			std::fill_n(_video, _bufferSize.width * _bufferSize.height, color.packed);
		}
#endif
	}

	inline void PixelMode::Pixel(const int32_t x, const int32_t y, const game::Color& color)
	{
#if defined(GAME_DIRECTX9)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			_video[y * _bufferSize.width + x] = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
		}
#endif
#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			_video[y * _bufferSize.width + x] = color.packed;
		}
#endif
	}

	inline void PixelMode::PixelClip(const int32_t x, const int32_t y, const game::Color& color)
	{
		if (x < 0 || y < 0) return;
		if (x > _bufferSize.width-1 || y > _bufferSize.height - 1) return;
#if defined(GAME_DIRECTX9)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			_video[y * _bufferSize.width + x] = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
		}
#endif
#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			_video[y * _bufferSize.width + x] = color.packed;
		}
#endif
	}
}

#undef PIXELMODEFVF