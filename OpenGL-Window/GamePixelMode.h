#pragma once
#if defined(GAME_OPENGL)
#include <gl/GL.h>
#endif
#if defined(GAME_DIRECTX9)
#include <d3d9.h>
#endif
#include "GameErrors.h"
#include "GameEngine.h"
#include "GameMath.h"
#include "GameTexture2D.h"

// Just a test to make sure defines are working
#if defined(GAME_USE_SHADERS)
#define USING_SHADERS
#endif

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
		void Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Color& color);
		void LineClip(int32_t x1, int32_t y1,  int32_t x2,  int32_t y2, const Color& color);
		void Circle(int32_t x, int32_t y, int32_t radius, const Color& color);
		void CircleClip(int32_t x, int32_t y, int32_t radius, const Color& color);
		void Rect(const Recti& rectangle, const Color& color);
		void RectClip(const Recti& rectangle, const Color& color);
	private:
		Texture2D _frameBuffer[2];
#if defined(GAME_OPENGL) & !defined(GAME_USE_SHADERS)
		uint32_t _compiledQuad;
#endif
#if defined(GAME_DIRECTX9)
		struct _vertex9
		{
			float_t x, y, z, rhw;    
			uint32_t color;    
			float_t u, v;
		};
		_vertex9 _QuadVertices9[6] =
		{
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 0.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 0.0f, 1.0f},

			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 1.0f, 1.0f},
			{0.0f, 0.0f, 0.0f, 1.0f, D3DCOLOR_ARGB(255,255, 255, 255), 0.0f, 1.0f}
		};
		LPDIRECT3DVERTEXBUFFER9 _vertexBuffer9;
#endif
#if defined(GAME_DIRECTX11)

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
		_video = nullptr;
		_currentBuffer = 0;
#if defined(GAME_OPENGL) & !defined(GAME_USE_SHADERS)
		_compiledQuad = 0;
#endif
#if defined(GAME_DIRECTX9)
		_vertexBuffer9 = nullptr;
#endif
#if defined(GAME_DIRECTX11)

#endif
	}

	inline PixelMode::~PixelMode()
	{
		if (_video != nullptr) delete[] _video;
#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			if (_vertexBuffer9)
			{
				_vertexBuffer9->Release();
				_vertexBuffer9 = nullptr;
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

#if defined(GAME_OPENGL) & !defined(GAME_USE_SHADERS)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			_compiledQuad = glGenLists(1);
		}
#endif
#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			enginePointer->d3d9Device->CreateVertexBuffer(6 * sizeof(_vertex9), 0, (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1), D3DPOOL_MANAGED, &_vertexBuffer9, NULL);
			if (_vertexBuffer9 == nullptr)
			{
				lastError = { GameErrors::GameDirectX9Specific, "Could not create vertex buffer for PixelMode." };
				return false;
			}
		}
#endif
#if defined (GAME_DIRECTX11)
		if (enginePointer->geIsUsing(GAME_DIRECTX11))
		{
			//enginePointer->d3d11Device->CreateBuffer()
		}
#endif

		// Scale the texture to window size
		_ScaleQuadToWindow();
		return true;
	}

	inline void PixelMode::_UpdateFrameBuffer()
	{
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			glBindTexture(GL_TEXTURE_2D, _frameBuffer[_currentBuffer].bind);

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _frameBuffer[_currentBuffer].width, _frameBuffer[_currentBuffer].height, GL_RGBA, game::systemInfo.gpuInfo.internalPixelType, (GLvoid*)_video);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
#endif
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			D3DLOCKED_RECT rect;
			unsigned char* test = (unsigned char*)_video;
			_frameBuffer[_currentBuffer].textureInterface9->LockRect(0, &rect, 0, 0);
			unsigned char* dest = static_cast<unsigned char*>(rect.pBits);
			memcpy(dest, test, sizeof(unsigned char) * _frameBuffer[_currentBuffer].width * _frameBuffer[_currentBuffer].height * 4);
			_frameBuffer[_currentBuffer].textureInterface9->UnlockRect(0);
		}
#endif
#if defined(GAME_DIRECTX11)
		if (enginePointer->geIsUsing(GAME_DIRECTX11))
		{
			D3D11_MAPPED_SUBRESOURCE data;// = nullptr;
			HRESULT hr;
			hr = enginePointer->d3d11Context->Map(_frameBuffer[_currentBuffer].textureInterface11, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
			memcpy(data.pData, (unsigned char*)_video, sizeof(unsigned char) * _frameBuffer[_currentBuffer].width * _frameBuffer[_currentBuffer].height * 4);
			enginePointer->d3d11Context->Unmap(_frameBuffer[_currentBuffer].textureInterface11, 0);
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

		// Pixel offset fix
		positionOfScaledTexture.x -= _frameBuffer[_currentBuffer].oneOverWidth;
		positionOfScaledTexture.y -= _frameBuffer[_currentBuffer].oneOverHeight;
		sizeOfScaledTexture.width -= _frameBuffer[_currentBuffer].oneOverWidth;
		sizeOfScaledTexture.height -= _frameBuffer[_currentBuffer].oneOverHeight;

#if defined(GAME_OPENGL) & !defined(GAME_USE_SHADERS)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{

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
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			VOID* pVoid = nullptr;  

			// tl
			_QuadVertices9[0].x = positionOfScaledTexture.x;
			_QuadVertices9[0].y = positionOfScaledTexture.y;
			// tr
			_QuadVertices9[1].x = sizeOfScaledTexture.width;
			_QuadVertices9[1].y = positionOfScaledTexture.y;
			// bl
			_QuadVertices9[2].x = positionOfScaledTexture.x;
			_QuadVertices9[2].y = sizeOfScaledTexture.height;

			// tr
			_QuadVertices9[3].x = sizeOfScaledTexture.width;
			_QuadVertices9[3].y = positionOfScaledTexture.y;
			// br
			_QuadVertices9[4].x = sizeOfScaledTexture.width;
			_QuadVertices9[4].y = sizeOfScaledTexture.height;
			// bl
			_QuadVertices9[5].x = positionOfScaledTexture.x;
			_QuadVertices9[5].y = sizeOfScaledTexture.height;

			// Copy vertices to the vertex buffer
			_vertexBuffer9->Lock(0, 0, (void**)&pVoid, 0);
			memcpy(pVoid, _QuadVertices9, sizeof(_QuadVertices9));
			_vertexBuffer9->Unlock();
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
#if defined(GAME_OPENGL) & !defined(GAME_USE_SHADERS)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _frameBuffer[_currentBuffer].bind);
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				glDisable(0x809D); // 0x809D is GL_MULTISAMPLE
			}
			glCallList(_compiledQuad);
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				glEnable(0x809D);
			}
			glDisable(GL_TEXTURE_2D);
		}
#endif
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			DWORD oldFVF = 0;
			IDirect3DBaseTexture9* activeTexture = 0;
			//enginePointer->d3d9Device->BeginScene();
			// Save current state
			enginePointer->d3d9Device->GetFVF(&oldFVF);
			enginePointer->d3d9Device->GetTexture(0, &activeTexture);

			// Disable multisampling if enabled
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				enginePointer->d3d9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
			}

			enginePointer->d3d9Device->SetTexture(0, _frameBuffer[_currentBuffer].textureInterface9);
			enginePointer->d3d9Device->SetFVF((D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1));
			enginePointer->d3d9Device->SetStreamSource(0, _vertexBuffer9, 0, sizeof(_vertex9));
			enginePointer->d3d9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

			// Restore previous state
			enginePointer->d3d9Device->SetFVF(oldFVF);
			enginePointer->d3d9Device->SetTexture(0, activeTexture);
			if (activeTexture)
			{
				activeTexture->Release();
			}

			// Renable multisampling if it was enabled
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				enginePointer->d3d9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
			}

			//enginePointer->d3d9Device->EndScene();
		}
		
#endif

		_currentBuffer++;
		if (_currentBuffer > 1) _currentBuffer = 0;

	}

	inline void PixelMode::Clear(const Color &color)
	{
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			std::fill_n(_video, _bufferSize.width * _bufferSize.height, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b));
		}
#endif
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			std::fill_n(_video, _bufferSize.width * _bufferSize.height, color.packed);
		}
#endif
	}

	inline void PixelMode::Pixel(const int32_t x, const int32_t y, const game::Color& color)
	{
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			_video[y * _bufferSize.width + x] = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
		}
#endif
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
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
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			_video[y * _bufferSize.width + x] = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
		}
#endif
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			_video[y * _bufferSize.width + x] = color.packed;
		}
#endif
	}

	inline void PixelMode::Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Color& color)
	{
		int32_t delta_x(x2 - x1);
		int32_t delta_y(y2 - y1);

		// if x1 == x2, then it does not matter what we set here
		int8_t const ix((delta_x > 0) - (delta_x < 0));
		delta_x = abs(delta_x) << 1;

		// if y1 == y2, then it does not matter what we set here
		int8_t const iy((delta_y > 0) - (delta_y < 0));
		delta_y = abs(delta_y) << 1;

		Pixel(x1, y1, color);

		if (delta_x >= delta_y)
		{
			// error may go below zero
			int32_t error(delta_y - (delta_x >> 1));

			while (x1 != x2)
			{
				if ((error >= 0) && (error || (ix > 0)))
				{
					error -= delta_x;
					y1 += iy;
				}
				// else do nothing

				error += delta_y;
				x1 += ix;

				Pixel(x1, y1, color);
			}
		}
		else
		{
			// error may go below zero
			int32_t error(delta_x - (delta_y >> 1));

			while (y1 != y2)
			{
				if ((error >= 0) && (error || (iy > 0)))
				{
					error -= delta_y;
					x1 += ix;
				}
				// else do nothing

				error += delta_x;
				y1 += iy;

				Pixel(x1, y1, color);
			}
		}
	}

	inline void PixelMode::LineClip(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Color& color)
	{
		struct _Clip
		{
			int clipTest(float_t p, float_t q, float_t* u1, float_t* u2) noexcept
			{
				float_t r;
				uint32_t retVal = TRUE;

				if (p < 0.0) {
					r = q / p;
					if (r > *u2)
						retVal = FALSE;
					else
						if (r > *u1)
							*u1 = r;
				}
				else
					if (p > 0.0) {
						r = q / p;
						if (r < *u1)
							retVal = FALSE;
						else if (r < *u2)
							*u2 = r;
					}
					else
						if (q < 0.0)
							retVal = FALSE;

				return (retVal);
			}
		};
		_Clip _clip;
		float_t u1 = 0.0;
		float_t u2 = 1.0;
		float_t dx = (float_t)(x2 - x1);
		float_t dy = 0;

		if (_clip.clipTest(-dx, (float_t)(x1 - 0), &u1, &u2))
			if (_clip.clipTest(dx, (float_t)(_bufferSize.width - 1 - x1), &u1, &u2)) {
				dy = (float_t)(y2 - y1);
				if (_clip.clipTest(-dy, (float_t)(y1 - 0), &u1, &u2))
					if (_clip.clipTest(dy, (float_t)(_bufferSize.height - 1 - y1), &u1, &u2)) {
						if (u2 < 1.0) {
							x2 = (int32_t)(x1 + u2 * dx);
							y2 = (int32_t)(y1 + u2 * dy);
						}
						if (u1 > 0.0) {
							x1 += (int32_t)(u1 * dx);
							y1 += (int32_t)(u1 * dy);
						}
						Line((int32_t)std::round(x1), (int32_t)std::round(y1), (int32_t)std::round(x2), (int32_t)std::round(y2), color);
					}
			}
	}

	inline void PixelMode::Circle(int32_t x, int32_t y, int32_t radius, const Color& color)
	{

	}

	inline void PixelMode::CircleClip(int32_t x, int32_t y, int32_t radius, const Color& color)
	{

	}

	inline void PixelMode::Rect(const Recti& rectangle, const Color& color)
	{
		// Top
		Line(rectangle.x, rectangle.y, rectangle.right, rectangle.y, color);
		// Bottom
		Line(rectangle.x, rectangle.bottom, rectangle.right, rectangle.bottom, color);
		// Left
		Line(rectangle.x, rectangle.y, rectangle.x, rectangle.bottom, color);
		// Right
		Line(rectangle.right, rectangle.y, rectangle.right, rectangle.bottom, color);
	}

	inline void PixelMode::RectClip(const Recti& rectangle, const Color& color)
	{
		// Top
		LineClip(rectangle.x, rectangle.y, rectangle.right, rectangle.y, color);
		// Bottom
		LineClip(rectangle.x, rectangle.bottom, rectangle.right, rectangle.bottom, color);
		// Left
		LineClip(rectangle.x, rectangle.y, rectangle.x, rectangle.bottom, color);
		// Right
		LineClip(rectangle.right, rectangle.y, rectangle.right, rectangle.bottom, color);
	}
}

#undef PIXELMODEFVF