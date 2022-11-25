#pragma once
#if defined(GAME_OPENGL)
#include <gl/GL.h>
#endif
#if defined(GAME_DIRECTX9)
#include <d3d9.h>
#endif

#include "GameEngine.h"
#include "GameErrors.h"
#include "GameMath.h"
#include "GameSpriteFont.h"
#include "GameTexture2D.h"

namespace game
{
	extern GameError lastError;
	extern Engine* enginePointer;

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();
		bool Initialize();
		void Begin();
		void Render();
		// Will draw entire texture to location x,y
		void Draw(const Texture2D &texture, const uint32_t x, const uint32_t y, const Color color);
		void Draw(const Texture2D& texture, const Pointi& position, const Color color);
		// Will draw a specified rectangle portion of a texture to location x,y
		void Draw(const Texture2D& texture, const Recti& destination, const Recti& portion, const Color& color);
		void DrawString(const SpriteFont &font, const std::string &Str, const int x, const int y, const Color& color);
		void End();
	private:
		static constexpr uint32_t _maxSprites = 1024;
		uint32_t _numberOfSpritesUsed;
		Texture2D _currentTexture;
		void _Enable2D();
		void _Disable2D();
		struct _spriteVertex
		{
			float_t x, y, z, rhw;
			uint32_t color;
			float_t u, v;
		};
		_spriteVertex* _spriteVertices;
#if defined(GAME_OPENGL)
		//float_t orthogonalMatrix[4][4] = { 0.0f };
#endif
#if defined(GAME_DIRECTX9)
		LPDIRECT3DVERTEXBUFFER9 _vertexBuffer;
		DWORD _savedFVF;
		DWORD _savedBlending;
		IDirect3DBaseTexture9* _savedTexture;
#endif
#if defined (GAME_DIRECTX11)
#endif
	};



	inline SpriteBatch::SpriteBatch()
	{
		_spriteVertices = nullptr;
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{

		}
#endif
#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			_vertexBuffer = nullptr;
			_savedFVF = 0;
			_savedBlending = 0;
			_savedTexture = nullptr;
		}
#endif
#if defined (GAME_DIRECTX11)
#endif

		_numberOfSpritesUsed = 0;
	}

	inline SpriteBatch::~SpriteBatch()
	{
#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{

		}
#endif
#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			if (_vertexBuffer)
			{
				_vertexBuffer->Release();
				_vertexBuffer = nullptr;
			}
			if (_spriteVertices)
			{
				delete[] _spriteVertices;
				_spriteVertices = nullptr;
			}
		}
#endif
#if defined (GAME_DIRECTX11)
#endif
	}

	inline bool SpriteBatch::Initialize()
	{
		_spriteVertices = new _spriteVertex[_maxSprites * 6];
		for (uint32_t vertex = 0; vertex < _maxSprites * 6; vertex++)
		{
			_spriteVertices[vertex].rhw = 1.0f;
			_spriteVertices[vertex].x = 0.0f;
			_spriteVertices[vertex].y = 0.0f;
			_spriteVertices[vertex].z = 0.0f;
			_spriteVertices[vertex].u = 0.0f;
			_spriteVertices[vertex].v = 0.0f;
			if (enginePointer->geIsUsing(GAME_DIRECTX9))
			{
				_spriteVertices[vertex].color = D3DCOLOR_ARGB(255, 255, 255, 255);
			}
			else if (enginePointer->geIsUsing(GAME_OPENGL))
			{
				_spriteVertices[vertex].color = Colors::White.packed;
			}
		}
#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{

		}
#endif
#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			enginePointer->d3d9Device->CreateVertexBuffer(_maxSprites * (uint32_t)6 * (uint32_t)sizeof(_spriteVertex), 0, (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1), D3DPOOL_MANAGED, &_vertexBuffer, NULL);
			if (_vertexBuffer == nullptr)
			{
				lastError = { GameErrors::GameDirectX9Specific, "Could not create vertex buffer for SpriteBatch." };
				return false;
			}
		}

		return true;
#endif
#if defined (GAME_DIRECTX11)
#endif
		return true;
	}

	inline void SpriteBatch::Begin()
	{
#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			// Save current state
			enginePointer->d3d9Device->GetFVF(&_savedFVF);
			enginePointer->d3d9Device->GetTexture(0, &_savedTexture);
			enginePointer->d3d9Device->GetRenderState(D3DRS_ALPHABLENDENABLE, &_savedBlending);

			// Disable multisampling if enabled
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				enginePointer->d3d9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
			}

			enginePointer->d3d9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			enginePointer->d3d9Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		}
#endif

#if defined (GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			glEnable(GL_TEXTURE_2D);
		}
#endif

	}

	inline void SpriteBatch::End()
	{
		if (_numberOfSpritesUsed > 0)
		{
			Render();
		}


#if defined (GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{

			// Restore previous state
			enginePointer->d3d9Device->SetFVF(_savedFVF);
			enginePointer->d3d9Device->SetTexture(0, _savedTexture);
			enginePointer->d3d9Device->SetRenderState(D3DRS_ALPHABLENDENABLE, _savedBlending);
			if (_savedTexture)
			{
				_savedTexture->Release();
			}

			// Renable multisampling if it was enabled
			if (enginePointer->_attributes.MultiSamples > 1)
			{
				enginePointer->d3d9Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
			}
		}
#endif
#if defined (GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			glBindTexture(GL_TEXTURE_2D,0);
			glDisable(GL_TEXTURE_2D);
		}
#endif
	}

	inline void SpriteBatch::Render()
	{
		if (_numberOfSpritesUsed == 0)
		{
			return;
		}

#if defined(GAME_DIRECTX9)
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			VOID* pVoid = nullptr;


			// Send sprite vertices to gpu
			_vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
			memcpy(pVoid, &_spriteVertices[0], sizeof(_spriteVertex) * 6 * _maxSprites);
			_vertexBuffer->Unlock();

			// Draw the sprites
			enginePointer->d3d9Device->SetTexture(0, _currentTexture.textureInterface9);
			enginePointer->d3d9Device->SetFVF((D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1));
			enginePointer->d3d9Device->SetStreamSource(0, _vertexBuffer, 0, sizeof(_spriteVertex));
			enginePointer->d3d9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, _numberOfSpritesUsed * 2);
		}
#endif

#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			Vector2i windowSize = enginePointer->geGetWindowSize();
			glBindTexture(GL_TEXTURE_2D, _currentTexture.bind);
			glBegin(GL_QUADS);
			// Pixel offset fix
			//positionOfScaledTexture.x -= _frameBuffer[_currentBuffer].oneOverWidth;
			//positionOfScaledTexture.y -= _frameBuffer[_currentBuffer].oneOverHeight;
			//sizeOfScaledTexture.width -= _frameBuffer[_currentBuffer].oneOverWidth;
			//sizeOfScaledTexture.height -= _frameBuffer[_currentBuffer].oneOverHeight;

			_spriteVertex *access = &_spriteVertices[0];
			for (uint32_t i = 0; i < _numberOfSpritesUsed; i++)
			{
				float_t r = (access->color & 255) / 255.0f;
				float_t g = (access->color >> 8 & 255) / 255.0f;
				float_t b = (access->color >> 16 & 255) / 255.0f;
				float_t a = (access->color >> 24 & 255) / 255.0f;
				// bl
				glTexCoord2f(access->u, access->v);
				glColor4f(r, g, b, a);
				access->x = (access->x * 2.0f / (float_t)windowSize.width) - 1.0f;
				access->y = (access->y * 2.0f / (float_t)windowSize.height) - 1.0f;
				access->y *= -1.0f;
				glVertex2f(access->x, access->y);
				access++;

				// br
				glTexCoord2f(access->u, access->v);
				glColor4f(r, g, b, a);
				access->x = (access->x * 2.0f / (float_t)windowSize.width) - 1.0f;
				access->y = (access->y * 2.0f / (float_t)windowSize.height) - 1.0f;
				access->y *= -1.0f;
				glVertex2f(access->x, access->y);
				access++;

				// tr
				glTexCoord2f(access->u, access->v);
				glColor4f(r, g, b, a);
				access->x = (access->x * 2.0f / (float_t)windowSize.width) - 1.0f;
				access->y = (access->y * 2.0f / (float_t)windowSize.height) - 1.0f;
				access->y *= -1.0f;
				glVertex2f(access->x, access->y);
				access++;

				// bl
				glTexCoord2f(access->u, access->v);
				glColor4f(r, g, b, a);
				access->x = (access->x * 2.0f / (float_t)windowSize.width) - 1.0f;
				access->y = (access->y * 2.0f / (float_t)windowSize.height) - 1.0f;
				access->y *= -1.0f;
				glVertex2f(access->x, access->y);
				access++;
			}


			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);

		}
#endif
		_numberOfSpritesUsed = 0;
	}

	inline void SpriteBatch::Draw(const Texture2D& texture, const Pointi& position, const Color color)
	{
		Draw(texture, position.x, position.y, color);
	}

	inline void SpriteBatch::Draw(const Texture2D& texture, const uint32_t x, const uint32_t y, const Color color)
	{
		if (_numberOfSpritesUsed + 1 > _maxSprites)
		{
			Render();
		}

#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			if (texture.bind != _currentTexture.bind)
			{
				Render();
				_currentTexture = texture;
			}
			_spriteVertex* access = &_spriteVertices[_numberOfSpritesUsed * 4];

			// bl
			access->x = (float_t)x;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 0.0f;
			access->v = 1.0f;
			access->color = color.packed;
			access++;

			// br
			access->x = (float_t)x + (float_t)texture.width;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 1.0f;
			access->v = 1.0f;
			access->color = color.packed;
			access++;

			// tr
			access->x = (float_t)x + (float_t)texture.width;
			access->y = (float_t)y;
			access->u = 1.0f;
			access->v = 0.0f;
			access->color = color.packed;
			access++;

			// tl
			access->x = (float_t)x;
			access->y = (float_t)y;
			access->u = 0.0f;
			access->v = 0.0f;
			access->color = color.packed;
			access++;

		}
#endif

#if defined(GAME_DIRECTX9)

		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			if (texture.textureInterface9 != _currentTexture.textureInterface9)
			{
				Render();
				_currentTexture = texture;
			}
			_spriteVertex* access = &_spriteVertices[_numberOfSpritesUsed * 6];
			// Top left
			access->x = (float_t)x;
			access->y = (float_t)y;
			access->u = 0.0f;
			access->v = 0.0f;
			access->color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
			access++;

			// Top right
			access->x = (float_t)x+(float_t)texture.width;
			access->y = (float_t)y;
			access->u = 1.0f;
			access->v = 0.0f;
			access->color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
			access++;

			// Bottom left
			access->x = (float_t)x;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 0.0f;
			access->v = 1.0f;
			access->color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
			access++;

			// Top right
			access->x = (float_t)x + (float_t)texture.width;
			access->y = (float_t)y;
			access->u = 1.0f;
			access->v = 0.0f;
			access->color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
			access++;

			// Bottom right
			access->x = (float_t)x + (float_t)texture.width;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 1.0f;
			access->v = 1.0f;
			access->color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
			access++;

			// Bottom left
			access->x = (float_t)x;
			access->y = (float_t)y + (float_t)texture.height;
			access->u = 0.0f;
			access->v = 1.0f;
			access->color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
			access++;
		}
#endif

		_numberOfSpritesUsed++;
	}

	inline void SpriteBatch::Draw(const Texture2D& texture, const Recti& destination, const Recti& portion, const Color& color)
	{
		if (_numberOfSpritesUsed + 1 > _maxSprites)
		{
			Render();
		}
#if defined(GAME_OPENGL)
		if (enginePointer->geIsUsing(GAME_OPENGL))
		{
			Vector2i windowSize = enginePointer->geGetWindowSize();
			if (texture.bind != _currentTexture.bind)
			{
				Render();
				_currentTexture = texture;
			}
			_spriteVertex* access = &_spriteVertices[_numberOfSpritesUsed * 4];

			// bl
			access->x = (float_t)destination.x - texture.oneOverWidth;
			access->y = (float_t)destination.bottom - texture.oneOverHeight;
			access->u = (float_t)portion.x * texture.oneOverWidth;
			access->v = (float_t)portion.bottom * texture.oneOverHeight;
			access->color = color.packed;
			access++;

			// br
			access->x = (float_t)destination.right - texture.oneOverWidth;
			access->y = (float_t)destination.bottom - texture.oneOverHeight;
			access->u = (float_t)portion.right * texture.oneOverWidth;
			access->v = (float_t)portion.bottom * texture.oneOverHeight;
			access->color = color.packed;
			access++;

			// tr
			access->x = (float_t)destination.right - texture.oneOverWidth;
			access->y = (float_t)destination.y - texture.oneOverHeight;
			access->u = (float_t)portion.right * texture.oneOverWidth;
			access->v = (float_t)portion.y * texture.oneOverHeight;
			access->color = color.packed;
			access++;

			// tl
			access->x = (float_t)destination.x - texture.oneOverWidth;
			access->y = (float_t)destination.y - texture.oneOverHeight;
			access->u = (float_t)portion.x * texture.oneOverWidth;
			access->v = (float_t)portion.y * texture.oneOverHeight;
			access->color = color.packed;
			access++;
		}
#endif


#if defined(GAME_DIRECTX9)
		if (texture.textureInterface9 != _currentTexture.textureInterface9)
		{
			Render();
			_currentTexture = texture;
		}
		if (enginePointer->geIsUsing(GAME_DIRECTX9))
		{
			_spriteVertex* access = &_spriteVertices[_numberOfSpritesUsed * 6];
			// Top left
			access->x = (float_t)destination.x - texture.oneOverWidth;
			access->y = (float_t)destination.y - texture.oneOverHeight;
			access->u = (float_t)portion.x * texture.oneOverWidth;
			access->v = (float_t)portion.y * texture.oneOverHeight;
			access->color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
			access++;

			// Top right
			access->x = (float_t)destination.right - texture.oneOverWidth;
			access->y = (float_t)destination.y - texture.oneOverHeight;
			access->u = (float_t)portion.right * texture.oneOverWidth;// 1.0f;
			access->v = (float_t)portion.y * texture.oneOverHeight;
			access->color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
			access++;

			// Bottom left
			access->x = (float_t)destination.x - texture.oneOverWidth;
			access->y = (float_t)destination.bottom - texture.oneOverHeight;
			access->u = (float_t)portion.x * texture.oneOverWidth;
			access->v = (float_t)portion.bottom * texture.oneOverHeight;
			access->color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
			access++;

			// Top right
			access->x = (float_t)destination.right - texture.oneOverWidth;
			access->y = (float_t)destination.y - texture.oneOverHeight;
			access->u = (float_t)portion.right * texture.oneOverWidth;// 1.0f;
			access->v = (float_t)portion.y * texture.oneOverHeight;
			access->color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
			access++;

			// Bottom right
			access->x = (float_t)destination.right - texture.oneOverWidth;
			access->y = (float_t)destination.bottom - texture.oneOverHeight;
			access->u = (float_t)portion.right * texture.oneOverWidth;
			access->v = (float_t)portion.bottom * texture.oneOverHeight;
			access->color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
			access++;

			// Bottom left
			access->x = (float_t)destination.x - texture.oneOverWidth;
			access->y = (float_t)destination.bottom - texture.oneOverHeight;
			access->u = (float_t)portion.x * texture.oneOverWidth;
			access->v = (float_t)portion.bottom * texture.oneOverHeight;
			access->color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
	}
#endif

		_numberOfSpritesUsed++;
	}
	
	void SpriteBatch::DrawString(const SpriteFont &font, const std::string& Str, const int x, const int y, const Color& color)
	{
		int CurX = x;
		int CurY = y;
		int Width, Height;
		Recti src, dest;
		short ch;

		for (unsigned int i = 0; i < Str.size(); ++i)
		{
			ch = Str[i];
			Width = font._characterSet.chars[ch].width;
			Height = font._characterSet.chars[ch].height;

			src.left = font._characterSet.chars[ch].x;
			src.top = font._characterSet.chars[ch].y;
			src.right = src.left + Width;
			src.bottom = (src.top + Height);

			dest.left = CurX + font._characterSet.chars[ch].xOffset;
			dest.top = CurY + font._characterSet.chars[ch].yOffset;
			dest.right = Width + dest.left;
			dest.bottom = Height + dest.top;

			Draw(font.Texture(), dest, src, color);

			CurX += font._characterSet.chars[ch].xAdvance;
	}
}

	inline void SpriteBatch::_Enable2D()
	{
#if defined(GAME_OPENGL)
		if (enginePointer->_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			//float Right = (float)enginePointer->geGetWindowSize().width;
			//float Left = 0.0;
			//float Top = 0.0f;
			//float Bottom = (float)enginePointer->geGetWindowSize().height;
			//float Near = -1.0f;
			//float Far = 1.0f;

			//orthogonalMatrix[0][0] = 2.0f / (Right - Left);

			//orthogonalMatrix[1][1] = 2.0f / (Top - Bottom);
			//orthogonalMatrix[2][2] = 2.0f / (Near - Far);

			//orthogonalMatrix[3][0] = (Left + Right) / (Left - Right);
			//orthogonalMatrix[3][1] = (Bottom + Top) / (Bottom - Top);

			//orthogonalMatrix[3][2] = (Far + Near) / (Near - Far);
		}
#endif
#if defined (GAME_DIRECTX9)
#endif
#if defined (GAME_DIRECTX11)
#endif
	}

	inline void SpriteBatch::_Disable2D()
	{
#if defined(GAME_DIRECTX9)

#endif
	}
}