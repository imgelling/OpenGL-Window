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
		void Clear(const Color &color) noexcept;
		void Pixel(const int32_t x, const int32_t y, const game::Color& color) noexcept;
		void PixelClip(const int32_t x, const int32_t y, const game::Color& color) noexcept;
		void Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Color& color) noexcept;
		void LineClip(int32_t x1, int32_t y1,  int32_t x2,  int32_t y2, const Color& color) noexcept;
		void Circle(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept;
		void CircleClip(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept;
		void CircleFilled(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept;
		void CircleFilledClip(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept;
		void Rect(const Recti& rectangle, const Color& color) noexcept;
		void RectClip(const Recti& rectangle, const Color& color) noexcept;
		void DrawHorizontalPillClip(const int32_t x, const int32_t y, const int32_t length, const int32_t radius, const game::Color& color) noexcept;
		
		Pointi GetScaledMousePosition() noexcept;
		Pointi GetPixelFrameBufferSize() noexcept;
	private:
		Texture2D _frameBuffer[2];
		Vector2f _oneOverScale;
		Vector2f _savedPositionOfScaledTexture;
		uint32_t* _video;
		Vector2i _bufferSize;
		Vector2i _windowSize;
		uint32_t _currentBuffer;
		void _UpdateFrameBuffer();
		void _ScaleQuadToWindow();
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
			D3D11_MAPPED_SUBRESOURCE data;
			HRESULT hr;
			hr = enginePointer->d3d11Context->Map(_frameBuffer[_currentBuffer].textureInterface11, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
			memcpy(data.pData, (unsigned char*)_video, sizeof(unsigned char) * _frameBuffer[_currentBuffer].width * _frameBuffer[_currentBuffer].height * 4);
			enginePointer->d3d11Context->Unmap(_frameBuffer[_currentBuffer].textureInterface11, 0);
		}
#endif
	}

	inline void PixelMode::_ScaleQuadToWindow()
	{
		float_t tempScale = 0.0f;
		Vector2f _scale;
		Vector2f _sizeOfScaledTexture;
		Vector2f _positionOfScaledTexture;

		if (_windowSize.height < _windowSize.width)
		{
			_scale.y = (float_t)_windowSize.height * _frameBuffer[_currentBuffer].oneOverHeight;
			tempScale = (float_t)_windowSize.width * _frameBuffer[_currentBuffer].oneOverWidth;
			if (tempScale > _scale.y)
			{
				_scale.x = _scale.y;
			}
			else
			{
				_scale.x = _scale.y = tempScale;
				_positionOfScaledTexture.y = ((_windowSize.height >> 1) - ((float_t)_frameBuffer[_currentBuffer].height * _scale.y / 2.0f));
			}
			_positionOfScaledTexture.x = ((_windowSize.width >> 1) - ((float_t)_frameBuffer[_currentBuffer].width * _scale.x / 2.0f));
		}
		else if (_windowSize.height > _windowSize.width)
		{
			_scale.x = (float_t)_windowSize.width * _frameBuffer[_currentBuffer].oneOverWidth;
			_scale.y = _scale.x;
			_positionOfScaledTexture.y = ((_windowSize.height >> 1) - ((float_t)_frameBuffer[_currentBuffer].height * _scale.y / 2.0f));
		}
		else
		{
			_scale = { 1.0f, 1.0f };
		}

		_oneOverScale.x = 1.0f / _scale.x;
		_oneOverScale.y = 1.0f / _scale.y;

		// Set the size of the scaled texture
		_sizeOfScaledTexture.width = _positionOfScaledTexture.x + (_frameBuffer[_currentBuffer].width * _scale.x);
		_sizeOfScaledTexture.height = _positionOfScaledTexture.y + (_frameBuffer[_currentBuffer].height * _scale.y);

		// Pixel offset fix
		_positionOfScaledTexture.x -= _frameBuffer[_currentBuffer].oneOverWidth;
		_positionOfScaledTexture.y -= _frameBuffer[_currentBuffer].oneOverHeight;
		_sizeOfScaledTexture.width -= _frameBuffer[_currentBuffer].oneOverWidth;
		_sizeOfScaledTexture.height -= _frameBuffer[_currentBuffer].oneOverHeight;

		_savedPositionOfScaledTexture = _positionOfScaledTexture;

#if defined(GAME_OPENGL) & !defined(GAME_USE_SHADERS)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{

			// Homoginize the scaled rect to -1 to 1 range using
			_positionOfScaledTexture.x = (_positionOfScaledTexture.x * 2.0f / (float_t)_windowSize.width) - 1.0f;
			_positionOfScaledTexture.y = (_positionOfScaledTexture.y * 2.0f / (float_t)_windowSize.height) - 1.0f;
			_sizeOfScaledTexture.width = ((float_t)_sizeOfScaledTexture.width * 2.0f / (float_t)_windowSize.width) - 1.0f;
			_sizeOfScaledTexture.height = ((float_t)_sizeOfScaledTexture.height * 2.0f / (float_t)_windowSize.height) - 1.0f;

			glNewList(_compiledQuad, GL_COMPILE);
			{
				glBegin(GL_QUADS);
				//bl
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(0, 1);
				glVertex2f(_positionOfScaledTexture.x, -_sizeOfScaledTexture.height);
				//br
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(1, 1);
				glVertex2f(_sizeOfScaledTexture.width, -_sizeOfScaledTexture.height);
				//tr
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(_sizeOfScaledTexture.width, -_positionOfScaledTexture.y);
				// tl
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(0, 0);
				glVertex2f(_positionOfScaledTexture.x, -_positionOfScaledTexture.y);

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
			_QuadVertices9[0].x = _positionOfScaledTexture.x;
			_QuadVertices9[0].y = _positionOfScaledTexture.y;
			// tr
			_QuadVertices9[1].x = _sizeOfScaledTexture.width;
			_QuadVertices9[1].y = _positionOfScaledTexture.y;
			// bl
			_QuadVertices9[2].x = _positionOfScaledTexture.x;
			_QuadVertices9[2].y = _sizeOfScaledTexture.height;

			// tr
			_QuadVertices9[3].x = _sizeOfScaledTexture.width;
			_QuadVertices9[3].y = _positionOfScaledTexture.y;
			// br
			_QuadVertices9[4].x = _sizeOfScaledTexture.width;
			_QuadVertices9[4].y = _sizeOfScaledTexture.height;
			// bl
			_QuadVertices9[5].x = _positionOfScaledTexture.x;
			_QuadVertices9[5].y = _sizeOfScaledTexture.height;

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

	inline void PixelMode::Clear(const Color &color) noexcept
	{
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			std::fill_n(_video, _bufferSize.width * _bufferSize.height, color.packedARGB);
		}
#endif
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			std::fill_n(_video, _bufferSize.width * _bufferSize.height, color.packedABGR);
		}
#endif
	}

	inline void PixelMode::Pixel(const int32_t x, const int32_t y, const game::Color& color) noexcept
	{
#if defined(_DEBUG)
		if (x < 0)
		{
			std::cout << "X < 0\n";
			return;
		}
		if (y < 0)
		{
			std::cout << "Y < 0\n";
			return;
		}
		if (x > _bufferSize.width - 1)
		{
			std::cout << "X > _bufferSize - 1\n";
			return;
		}
		if (y > _bufferSize.height - 1)
		{
			std::cout << "Y > _bufferSize - 1\n";
			return;
		}
#endif
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			_video[y * _bufferSize.width + x] = color.packedARGB;
			return;
		}
#endif
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			_video[y * _bufferSize.width + x] = color.packedABGR;
			return;
		}
#endif
	}

	inline void PixelMode::PixelClip(const int32_t x, const int32_t y, const game::Color& color) noexcept
	{
		if (x < 0 || y < 0) return;
		if (x > _bufferSize.width-1 || y > _bufferSize.height - 1) return;
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			_video[y * _bufferSize.width + x] = color.packedARGB;
		}
#endif
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			_video[y * _bufferSize.width + x] = color.packedABGR;
		}
#endif
	}

	inline void PixelMode::Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Color& color) noexcept
	{
		int32_t delta_x(x2 - x1);
		int32_t delta_y(y2 - y1);

		// if x1 == x2, then it does not matter what we set here
		const int8_t ix((delta_x > 0) - (delta_x < 0));
		delta_x = abs(delta_x) << 1;

		// if y1 == y2, then it does not matter what we set here
		const int8_t iy((delta_y > 0) - (delta_y < 0));
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

	inline void PixelMode::LineClip(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Color& color) noexcept
	{
		//Liang - Barsky Algorithm

		struct _Clip
		{
			inline int clipTest(float_t p, float_t q, float_t* u1, float_t* u2) noexcept
			{
				float_t r(0.0f);

				if (p < 0.0) {
					r = q / p;
					if (r > *u2)
						return FALSE;
					else
						if (r > *u1)
							*u1 = r;
				}
				else
					if (p > 0.0) {
						r = q / p;
						if (r < *u1)
							return FALSE;
						else if (r < *u2)
							*u2 = r;
					}
					else
						if (q < 0.0)
							return FALSE;

				return TRUE;
			}
		};

		_Clip _clip;
		float_t u1(0.0f);
		float_t u2(1.0f);
		float_t dx((float_t)(x2 - x1));
		float_t dy(0);
		float_t x1f((float_t)x1);
		float_t x2f((float_t)x2);
		float_t y1f((float_t)y1);
		float_t y2f((float_t)y2);

		if (_clip.clipTest(-dx, x1f - 0, &u1, &u2))
			if (_clip.clipTest(dx, _bufferSize.width - 1 - x1f, &u1, &u2)) {
				dy = y2f - y1f;
				if (_clip.clipTest(-dy, y1f, &u1, &u2))
					if (_clip.clipTest(dy, _bufferSize.height - 1 - y1f, &u1, &u2)) {
						if (u2 < 1.0) {
							x2f = x1f + u2 * dx;
							y2f = y1f + u2 * dy;
						}
						if (u1 > 0.0) {
							x1f += u1 * dx;
							y1f += u1 * dy;
						}
						Line((int32_t)std::round(x1f), (int32_t)std::round(y1f), (int32_t)std::round(x2f), (int32_t)std::round(y2f), color);
					}
			}
	}

	inline void PixelMode::Circle(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept
	{
		if (radius < 0 || x < -radius || y < -radius || x - _bufferSize.width > radius || y - _bufferSize.height > radius)
			return;

		if (radius > 0)
		{
			int x0 = 0;
			int y0 = radius;
			int d = 3 - 2 * radius;

			while (y0 >= x0) // only formulate 1/8 of circle
			{
				// Draw even octants
				Pixel(x + x0, y - y0, color);// Q6 - upper right right
				Pixel(x + y0, y + x0, color);// Q4 - lower lower right
				Pixel(x - x0, y + y0, color);// Q2 - lower left left
				Pixel(x - y0, y - x0, color);// Q0 - upper upper left
				if (x0 != 0 && x0 != y0)
				{
					Pixel(x + y0, y - x0, color);// Q7 - upper upper right
					Pixel(x + x0, y + y0, color);// Q5 - lower right right
					Pixel(x - y0, y + x0, color);// Q3 - lower lower left
					Pixel(x - x0, y - y0, color);// Q1 - upper left left
				}

				if (d < 0)
					d += 4 * x0++ + 6;
				else
					d += 4 * (x0++ - y0--) + 10;
			}
		}
		else
			Pixel(x, y, color);
	}

	inline void PixelMode::CircleClip(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept
	{
		if (radius < 0 || x < -radius || y < -radius || x - _bufferSize.width > radius || y - _bufferSize.height > radius)
			return;

		if (radius > 0)
		{
			int32_t x0(0);
			int32_t y0(radius);
			int32_t d(3 - 2 * radius);

			while (y0 >= x0) // only formulate 1/8 of circle
			{
				// Draw even octants
				PixelClip(x + x0, y - y0, color);// Q6 - upper right right
				PixelClip(x + y0, y + x0, color);// Q4 - lower lower right
				PixelClip(x - x0, y + y0, color);// Q2 - lower left left
				PixelClip(x - y0, y - x0, color);// Q0 - upper upper left
				if (x0 != 0 && x0 != y0)
				{
					PixelClip(x + y0, y - x0, color);// Q7 - upper upper right
					PixelClip(x + x0, y + y0, color);// Q5 - lower right right
					PixelClip(x - y0, y + x0, color);// Q3 - lower lower left
					PixelClip(x - x0, y - y0, color);// Q1 - upper left left
				}

				if (d < 0)
					d += 4 * x0++ + 6;
				else
					d += 4 * (x0++ - y0--) + 10;
			}
		}
		else
			PixelClip(x, y, color);
	}

	inline void PixelMode::CircleFilled(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept
	{
		if (radius < 0 || x < -radius || y < -radius || x - _bufferSize.width > radius || y - _bufferSize.height > radius)
			return;

		if (radius > 0)
		{
			int32_t x0 = 0;
			int32_t y0 = radius;
			int32_t d = 3 - 2 * radius;

			auto drawline = [&](int32_t sx, int32_t ex, int32_t y)
			{
				for (int32_t x = sx; x <= ex; x++)
					Pixel(x, y, color);
			};

			while (y0 >= x0)
			{
				drawline(x - y0, x + y0, y - x0);
				if (x0 > 0)	drawline(x - y0, x + y0, y + x0);

				if (d < 0)
					d += 4 * x0++ + 6;
				else
				{
					if (x0 != y0)
					{
						drawline(x - x0, x + x0, y - y0);
						drawline(x - x0, x + x0, y + y0);
					}
					d += 4 * (x0++ - y0--) + 10;
				}
			}
		}
		else
			Pixel(x, y, color);
	}

	inline void PixelMode::CircleFilledClip(int32_t x, int32_t y, int32_t radius, const Color& color) noexcept
	{
		if (radius < 0 || x < -radius || y < -radius || x - _bufferSize.width > radius || y - _bufferSize.height > radius)
			return;

		if (radius > 0)
		{
			int32_t x0 = 0;
			int32_t y0 = radius;
			int32_t d = 3 - 2 * radius;

			auto drawline = [&](int32_t sx, int32_t ex, int32_t y)
			{
				if (sx < 0) sx = 0;
				if (ex > _bufferSize.width - 1) ex = _bufferSize.width - 1;
				if (y < 0) y = 0;
				if (y > _bufferSize.height - 1) y = _bufferSize.height - 1;
				for (int x = sx; x <= ex; x++)
					Pixel(x, y, color);
			};

			while (y0 >= x0)
			{
				drawline(x - y0, x + y0, y - x0);
				if (x0 > 0)	drawline(x - y0, x + y0, y + x0);

				if (d < 0)
					d += 4 * x0++ + 6;
				else
				{
					if (x0 != y0)
					{
						drawline(x - x0, x + x0, y - y0);
						drawline(x - x0, x + x0, y + y0);
					}
					d += 4 * (x0++ - y0--) + 10;
				}
			}
		}
		else
			Pixel(x, y, color);
	}

	inline void PixelMode::Rect(const Recti& rectangle, const Color& color) noexcept
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

	inline void PixelMode::RectClip(const Recti& rectangle, const Color& color) noexcept
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

	inline void PixelMode::DrawHorizontalPillClip(const int32_t x, const int32_t y, const int32_t length, const int32_t radius, const game::Color& color) noexcept
	{
		int32_t calculatedLength = length - (radius * 2);

		if (radius < 0 || x < -length || y < -radius || x - _bufferSize.width > length || y - _bufferSize.height > radius)
			return;

		if (radius > 0)
		{
			int32_t x0 = 0;
			int32_t y0 = radius;
			int32_t d = 3 - 2 * radius;

			while (y0 >= x0)
			{
				LineClip(x - y0, y - x0, x + y0 + calculatedLength, y - x0, color);
				if (x0 > 0)
				{
					LineClip(x - y0, y + x0, x + y0 + calculatedLength, y + x0, color);
				}

				if (d < 0)
					d += 4 * x0++ + 6;
				else
				{
					if (x0 != y0)
					{
						LineClip(x - x0, y - y0, x + x0 + calculatedLength, y - y0, color);
						LineClip(x - x0, y + y0, x + x0 + calculatedLength, y + y0, color);
					}
					d += 4 * (x0++ - y0--) + 10;
				}
			}
		}
		else
			Pixel(x, y, color);
	}

	inline Pointi PixelMode::GetScaledMousePosition() noexcept
	{
		Pointi scaledMouseCoords = enginePointer->geMouse.GetPosition();

		scaledMouseCoords.x = (int32_t)(scaledMouseCoords.x - _savedPositionOfScaledTexture.x);
		scaledMouseCoords.y = (int32_t)(scaledMouseCoords.y - _savedPositionOfScaledTexture.y);
		scaledMouseCoords.x = (int32_t)(scaledMouseCoords.x * _oneOverScale.x);
		scaledMouseCoords.y = (int32_t)(scaledMouseCoords.y * _oneOverScale.y);
		return scaledMouseCoords;
	}

	inline Pointi PixelMode::GetPixelFrameBufferSize() noexcept
	{
		return { _bufferSize.width, _bufferSize.height };
	}
}

#undef PIXELMODEFVF